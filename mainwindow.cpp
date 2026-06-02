#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QSplitter>
#include <QGroupBox>
#include <QFont>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Chrono Debug");
    resize(1200, 800);
    compile_process = new QProcess(this);
    run_process = new QProcess(this);
    my_timer = new QElapsedTimer();
    compile_time_ms = 0;
    run_time_ms = 0;
    setup_ui();

    // Set placeholder text (grayed out hint)
    code_editor->setPlaceholderText(
        "#include <iostream>\n"
        "#include <vector>\n"
        "using namespace std;\n\n"
        "void slowFunction() {\n"
        "    long long sum = 0;\n"
        "    for (long long i = 0; i < 1000000000; i++) {\n"
        "        sum += i;\n"
        "    }\n"
        "}\n\n"
        "void fastFunction() {\n"
        "    int result = 50 * 51 / 2;\n"
        "}\n\n"
        "int main() {\n"
        "    cout << \"ChronoDebug Pro Demo\" << endl;\n"
        "    fastFunction();\n"
        "    slowFunction();\n"
        "    return 0;\n"
        "}\n"
        );
}

MainWindow::~MainWindow()
{
    // Kill any running process when app closes
    run_process->kill();
    compile_process->kill();
    delete my_timer;
}

void MainWindow::setup_ui()
{
    QWidget *main_widget = new QWidget(this);
    setCentralWidget(main_widget);

    QVBoxLayout *main_layout = new QVBoxLayout(main_widget);
    QHBoxLayout *button_row = new QHBoxLayout();

    QPushButton *run_btn = new QPushButton("▶ Run Code");
    run_btn->setStyleSheet("background-color: #4CAF50; color: white; font-size: 14px; padding: 10px; font-weight: bold;");
    connect(run_btn, &QPushButton::clicked, this, &MainWindow::run_user_code);
    button_row->addWidget(run_btn);

    QPushButton *clear_btn = new QPushButton("🗑 Clear");
    clear_btn->setStyleSheet("background-color: #f44336; color: white; font-size: 14px; padding: 10px;");
    connect(clear_btn, &QPushButton::clicked, this, &MainWindow::clear_everything);
    button_row->addWidget(clear_btn);

    QPushButton *save_btn = new QPushButton("💾 Save Code");
    save_btn->setStyleSheet("background-color: #2196F3; color: white; font-size: 14px; padding: 10px;");
    connect(save_btn, &QPushButton::clicked, this, &MainWindow::save_code_to_file);
    button_row->addWidget(save_btn);

    QPushButton *open_btn = new QPushButton("📂 Open Code");
    open_btn->setStyleSheet("background-color: #FF9800; color: white; font-size: 14px; padding: 10px;");
    connect(open_btn, &QPushButton::clicked, this, &MainWindow::open_code_from_file);
    button_row->addWidget(open_btn);

    QPushButton *example_btn = new QPushButton("📖 Example");
    example_btn->setStyleSheet("background-color: #9C27B0; color: white; font-size: 14px; padding: 10px;");
    connect(example_btn, &QPushButton::clicked, this, &MainWindow::load_example_code);
    button_row->addWidget(example_btn);

    QPushButton *report_btn = new QPushButton("📄 Save Report");
    report_btn->setStyleSheet("background-color: #607D8B; color: white; font-size: 14px; padding: 10px;");
    connect(report_btn, &QPushButton::clicked, this, &MainWindow::save_report_to_file);
    button_row->addWidget(report_btn);

    main_layout->addLayout(button_row);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    QGroupBox *editor_box = new QGroupBox("Type Your C++ Code:");
    QVBoxLayout *editor_layout = new QVBoxLayout(editor_box);

    code_editor = new QPlainTextEdit();
    code_editor->setStyleSheet("background-color: #1e1e1e; color: #d4d4d4; font-family: Consolas; font-size: 13px; padding: 10px;");

    code_editor->setFont(QFont("Consolas", 12));

    editor_layout->addWidget(code_editor);
    splitter->addWidget(editor_box);

    QWidget *right_panel = new QWidget();
    QVBoxLayout *right_layout = new QVBoxLayout(right_panel);

    QGroupBox *output_box = new QGroupBox("OUTPUT:");
    QVBoxLayout *output_layout = new QVBoxLayout(output_box);

    output_display = new QTextEdit();
    output_display->setReadOnly(true);
    output_display->setStyleSheet("background-color: #f5f5f5; color: #000; font-family: Consolas; font-size: 12px; padding: 10px;");

    output_layout->addWidget(output_display);
    right_layout->addWidget(output_box);

    QGroupBox *timeline_box = new QGroupBox("TIMELINE:");
    QVBoxLayout *timeline_layout = new QVBoxLayout(timeline_box);

    timeline_display = new QTextEdit();
    timeline_display->setReadOnly(true);
    timeline_display->setMaximumHeight(200);
    timeline_display->setStyleSheet("background-color: #ffffff; color: #000; font-family: Consolas; font-size: 11px; padding: 10px;");

    timeline_layout->addWidget(timeline_display);
    right_layout->addWidget(timeline_box);

    splitter->addWidget(right_panel);

    QList<int> sizes;
    sizes.append(600);
    sizes.append(600);
    splitter->setSizes(sizes);

    main_layout->addWidget(splitter);
}

void MainWindow::run_user_code()
{
    user_code = code_editor->toPlainText();
    user_code = user_code.trimmed();
    if(user_code.isEmpty())
    {
        QMessageBox::warning(this, "Empty", "Please write some code first!");
        return;
    }
    output_display->clear();
    timeline_display->clear();
    output_display->setHtml("<b style='color: blue;'>⏳ Compiling your code...</b>");
    compile_the_code();
}

void MainWindow::compile_the_code()
{
    // Kill any previous running program first
    run_process->kill();
    run_process->waitForFinished(500);

    QString temp_folder = QDir::tempPath();
    QString code_file = temp_folder + "/my_code.cpp";
    QString exe_file = temp_folder + "/my_code.exe";
    QFile::remove(exe_file);
    QFile file(code_file);
    bool opened = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(opened == false)
    {
        output_display->setHtml("<b style='color: red;'>❌ ERROR: Cannot create file!</b>");
        return;
    }
    file.write(user_code.toUtf8());
    file.close();
    my_timer->start();
    QString compiler_path = "D:/Qt/Tools/mingw1310_64/bin/g++.exe";
    QStringList arguments;
    arguments.append(code_file);
    arguments.append("-o");
    arguments.append(exe_file);
    compile_process->setWorkingDirectory(temp_folder);
    connect(compile_process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            [this, exe_file](int exit_code)
            {
                compile_time_ms = my_timer->elapsed();
                QString output = compile_process->readAllStandardOutput();
                QString errors = compile_process->readAllStandardError();
                compile_errors = output + errors;
                bool success = (exit_code == 0) && QFile::exists(exe_file);
                if(success)
                {
                    QString message = "";
                    message = message + "<b style='color: green;'>✓ Compilation SUCCESS!</b><br><br>";
                    message = message + "<b>Compile Time:</b> " + QString::number(compile_time_ms) + " ms<br><br>";
                    message = message + "<b style='color: blue;'>⏳ Running program...</b>";
                    output_display->setHtml(message);
                    execute_the_program();
                }
                else
                {
                    QString message = "";
                    message = message + "<b style='color: red;'>❌ COMPILATION FAILED!</b><br><br>";
                    message = message + "<b>Compile Time:</b> " + QString::number(compile_time_ms) + " ms<br><br>";
                    message = message + "<b>Errors:</b><br>";
                    message = message + "<pre style='color: red; background: #ffe6e6; padding: 10px;'>" + compile_errors.toHtmlEscaped() + "</pre>";
                    output_display->setHtml(message);
                    QString timeline = "";
                    timeline = timeline + "<b style='color: red;'>⏱ TIMELINE:</b><br>";
                    timeline = timeline + "Compilation: <span style='color: red;'>FAILED</span> (" + QString::number(compile_time_ms) + " ms)";
                    timeline_display->setHtml(timeline);
                }
                compile_process->disconnect();
            });
    compile_process->start(compiler_path, arguments);
}

void MainWindow::execute_the_program()
{
    QString temp_folder = QDir::tempPath();
    QString exe_file = temp_folder + "/my_code.exe";
    my_timer->restart();
    run_process->start(exe_file);
    run_process->waitForFinished(5000);
    run_time_ms = my_timer->elapsed();
    program_output = run_process->readAllStandardOutput();
    QString program_errors = run_process->readAllStandardError();
    int program_exit_code = run_process->exitCode();
    QString speed_color;
    QString speed_text;
    QString speed_emoji;
    if(run_time_ms < 160)
    {
        speed_color = "green";
        speed_text = "FAST";
        speed_emoji = "⚡";
    }
    else if(run_time_ms < 300)
    {
        speed_color = "orange";
        speed_text = "MEDIUM";
        speed_emoji = "⚙️";
    }
    else
    {
        speed_color = "red";
        speed_text = "SLOW";
        speed_emoji = "🐌";
    }

    QString result = "";
    result = result + "<b style='color: green;'>✓ Compilation SUCCESS!</b><br>";
    result = result + "<b>Compile Time:</b> " + QString::number(compile_time_ms) + " ms<br><br>";
    if(program_exit_code == 0)
    {
        result = result + "<b style='color: green;'>✓ Execution SUCCESS!</b><br>";
        result = result + "<b>Run Time:</b> <span style='color: " + speed_color + "; font-weight: bold; font-size: 14px;'>";
        result = result + QString::number(run_time_ms) + " ms - " + speed_emoji + " " + speed_text + "</span><br><br>";
        result = result + "<b>Program Output:</b><br>";
        if(program_output.isEmpty())
        {
            result = result + "<pre style='background: #e8f5e9; padding: 10px; border-left: 4px solid green;'>(no output)</pre>";
        }
        else
        {
            result = result + "<pre style='background: #e8f5e9; padding: 10px; border-left: 4px solid green;'>";
            result = result + program_output.toHtmlEscaped() + "</pre>";
        }
    }
    else
    {
        result = result + "<b style='color: red;'>❌ Execution FAILED!</b><br>";
        result = result + "<b>Exit Code:</b> " + QString::number(program_exit_code) + "<br>";
        result = result + "<b>Run Time:</b> " + QString::number(run_time_ms) + " ms<br><br>";

        if(program_errors.isEmpty() == false)
        {
            result = result + "<b>Error:</b><br>";
            result = result + "<pre style='background: #ffebee; padding: 10px; color: red;'>" + program_errors.toHtmlEscaped() + "</pre>";
        }
    }
    output_display->setHtml(result);
    qint64 total_time = compile_time_ms + run_time_ms;
    if(total_time == 0)
    {
        total_time = 1;
    }
    int compile_percent = (compile_time_ms * 100) / total_time;
    int run_percent = (run_time_ms * 100) / total_time;
    QString run_bar_color;
    if(run_time_ms < 160)
    {
        run_bar_color = "#4CAF50"; //green bar
    }
    else if(run_time_ms < 300)
    {
        run_bar_color = "#FFC107";//yellow bar
    }
    else
    {
        run_bar_color = "#f44336"; //red bar
    }
    int compile_blocks = compile_percent / 5;
    int run_blocks = run_percent / 5;
    if(compile_blocks < 1)
    {
        compile_blocks = 1;
    }
    if(run_blocks < 1)
    {
        run_blocks = 1;
    }
    QString compile_bar = "";
    for(int i = 0; i < compile_blocks; i++)
    {
        compile_bar = compile_bar + "█";
    }
    QString run_bar = "";
    for(int i = 0; i < run_blocks; i++)
    {
        run_bar = run_bar + "█";
    }

    QString timeline = "";
    timeline = timeline + "<div style='font-family: Consolas; font-size: 12px;'>";
    timeline = timeline + "<b style='color: #ff6f00; font-size: 14px;'>⏱ EXECUTION TIMELINE</b><br><br>";
    timeline = timeline + "<b>📊 Compilation:</b> " + QString::number(compile_time_ms) + " ms (" + QString::number(compile_percent) + "%) - SUCCESS<br>";
    timeline = timeline + "<span style='color: #4CAF50; font-size: 16px;'>" + compile_bar + "</span><br><br>";
    timeline = timeline + "<b>⚡ Execution:</b> " + QString::number(run_time_ms) + " ms (" + QString::number(run_percent) + "%) - ";
    timeline = timeline + "<span style='color: " + speed_color + "; font-weight: bold;'>" + speed_text + "</span><br>";
    timeline = timeline + "<span style='color: " + run_bar_color + "; font-size: 16px;'>" + run_bar + "</span><br><br>";
    timeline = timeline + "<b>⏰ Total Time:</b> " + QString::number(total_time) + " ms<br><br>";
    timeline = timeline + "<hr>";
    timeline = timeline + "<b>Performance Rating Guide:</b><br>";
    timeline = timeline + "<span style='color: #4CAF50;'>⚡ FAST:</span> &lt; 160ms <span style='color: #4CAF50;'>█████</span> GREEN<br>";
    timeline = timeline + "<span style='color: #FFC107;'>⚙️ MEDIUM:</span> 160-300ms <span style='color: #FFC107;'>█████</span> YELLOW<br>";
    timeline = timeline + "<span style='color: #f44336;'>🐌 SLOW:</span> &gt; 300ms <span style='color: #f44336;'>█████</span> RED";
    timeline = timeline + "</div>";

    timeline_display->setHtml(timeline);
}

void MainWindow::clear_everything()
{
    code_editor->clear();
    output_display->clear();
    timeline_display->clear();
}

void MainWindow::save_code_to_file()
{
    QString file_name = QFileDialog::getSaveFileName(this, "Save Code", "my_code.cpp", "C++ Files (*.cpp)");
    if(file_name.isEmpty())
    {
        return;
    }
    QFile file(file_name);
    bool opened = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(opened == false)
    {
        QMessageBox::critical(this, "Error", "Cannot save!");
        return;
    }
    QString code = code_editor->toPlainText();
    file.write(code.toUtf8());
    file.close();
    QMessageBox::information(this, "Saved", "Code saved!");
}

void MainWindow::open_code_from_file()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Open Code", "", "C++ Files (*.cpp)");
    if(file_name.isEmpty())
    {
        return;
    }
    QFile file(file_name);
    bool opened = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(opened == false)
    {
        QMessageBox::critical(this, "Error", "Cannot open!");
        return;
    }
    QString content = file.readAll();
    file.close();
    code_editor->setPlainText(content);
}

void MainWindow::load_example_code()
{
    QString example = "";
    example = example + "#include <iostream>\n";
    example = example + "using namespace std;\n";
    example = example + "\n";
    example = example + "int main() {\n";
    example = example + "    cout << \"Developed by Sibghatullah!\" << endl;\n";
    example = example + "    \n";
    example = example + "    int x = 10;\n";
    example = example + "    int y = 20;\n";
    example = example + "    int sum = x + y;\n";
    example = example + "    \n";
    example = example + "    cout << \"Sum of \" << x << \" and \" << y << \" is: \" << sum << endl;\n";
    example = example + "    \n";
    example = example + "    return 0;\n";
    example = example + "}\n";

    code_editor->setPlainText(example);
    QMessageBox::information(this, "Loaded", "Example loaded! Click Run to execute.");
}

void MainWindow::save_report_to_file()
{
    if(user_code.isEmpty())
    {
        QMessageBox::warning(this, "No Data", "Run some code first!");
        return;
    }
    QString speed_rating;
    if(run_time_ms < 160)
    {
        speed_rating = "FAST (GREEN)";
    }
    else if(run_time_ms < 300)
    {
        speed_rating = "MEDIUM (YELLOW)";
    }
    else
    {
        speed_rating = "SLOW (RED)";
    }
    QString file_name = QFileDialog::getSaveFileName(this, "Save Report", "chrono_report.txt", "Text Files (*.txt)");
    if(file_name.isEmpty())
    {
        return;
    }
    QFile file(file_name);
    bool opened = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(opened == false)
    {
        QMessageBox::critical(this, "Error", "Cannot save report!");
        return;
    }
    qint64 total = compile_time_ms + run_time_ms;
    QString report = "";
    report = report + "========================================\n";
    report = report + "  CHRONO DEBUG PRO - EXECUTION REPORT\n";
    report = report + "========================================\n\n";
    report = report + "Date: " + QDateTime::currentDateTime().toString() + "\n\n";
    report = report + "CODE:\n";
    report = report + "----------------------------------------\n";
    report = report + user_code + "\n\n";
    report = report + "========================================\n";
    report = report + "PERFORMANCE:\n";
    report = report + "========================================\n";
    report = report + "Compilation Time: " + QString::number(compile_time_ms) + " ms\n";
    report = report + "Execution Time:   " + QString::number(run_time_ms) + " ms\n";
    report = report + "Total Time:       " + QString::number(total) + " ms\n";
    report = report + "Speed Rating:     " + speed_rating + "\n";
    report = report + "\n";
    report = report + "Performance Guide:\n";
    report = report + "  FAST (GREEN):   < 160ms\n";
    report = report + "  MEDIUM (YELLOW): 161-300ms\n";
    report = report + "  SLOW (RED):     > 300ms\n";
    report = report + "========================================\n";
    report = report + "OUTPUT:\n";
    report = report + "----------------------------------------\n";
    report = report + program_output + "\n";
    report = report + "========================================\n";

    file.write(report.toUtf8());
    file.close();
    QMessageBox::information(this, "Saved", "Report saved successfully!");
}
