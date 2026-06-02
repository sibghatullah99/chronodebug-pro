#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QProcess>
#include <QElapsedTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // button click functions
    void run_user_code();
    void clear_everything();
    void save_code_to_file();
    void open_code_from_file();
    void save_report_to_file();
    void load_example_code();

private:
    //  text boxes and areas
    QPlainTextEdit *code_editor;
    QTextEdit *output_display;
    QTextEdit *timeline_display;

    // for running the code
    QProcess *compile_process;
    QProcess *run_process;
    QElapsedTimer *my_timer;

    // storing data
    QString user_code;
    QString compile_errors;
    QString program_output;
    qint64 compile_time_ms;
    qint64 run_time_ms;

    //  functions required for help
    void setup_ui();
    void compile_the_code();
    void execute_the_program();
};

#endif
