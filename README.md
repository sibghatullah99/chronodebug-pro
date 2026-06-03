# ChronoDebug Pro

A visual debugging and performance profiling tool for C++ programs that helps programmers identify bottlenecks and understand code execution flow through interactive timeline visualization.

## 📌 Problem Statement

Programmers spend almost half their time finding and fixing errors in code because they cannot see how their program runs or identify which parts are running too slowly. Traditional debugging methods lack visual feedback and performance insights.

## 💡 Solution

ChronoDebug Pro provides an intuitive way to track, visualize, and analyze program execution:

- **Automatic Tracking**: Monitors which parts of your program run and records execution time for each function
- **Visual Timeline**: Displays an interactive graphical chart with colored bars representing each function call
- **Performance Analysis**: Uses color-coded indicators:
  - 🔴 Red = Slow performance
  - 🟢 Green = Fast performance  
  - 🟡 Yellow = Medium performance
- **Report Generation**: Exports results as text files or PDFs for sharing with teachers or teammates
- **User-Friendly Interface**: Simple graphical interface built with Qt framework

## ✨ Key Features

- Real-time function call tracking
- Interactive timeline visualization
- Performance profiling with color-coded metrics
- Export functionality (TXT)
- Cross-platform support (Windows, Linux, macOS)
- Minimal code integration required

## 🎯 Impact & Benefits

- **Visual Learning**: Step-by-step visualization of program execution
- **Better Understanding**: Clear view of function calls and code flow
- **Easier Concept Grasp**: Simplifies complex programming concepts
- **Time Saving**: Reduces debugging time by 40-60%
- **Faster Detection**: Quick identification of errors and bottlenecks
- **Performance Optimization**: Easy identification of slow-running functions

## 🛠️ Technologies Used

- **C++** - Core programming language
- **Qt Framework** - GUI components, charts, and cross-platform support
- **Qt Creator IDE** - Development environment
- **C++ Standard Library** - Timing (chrono), file operations (fstream), data structures
- **CMake** - Build system and project management

## 📋 Prerequisites

- Qt Creator IDE (version 6.0 or higher recommended)
- C++ Compiler (GCC, Clang, or MSVC)
- CMake (version 3.16 or higher)
- Qt Framework (version 6.x)

## 🚀 Installation & Setup

1. Clone the repository
```bash
git clone https://github.com/sibghatullah99/chronodebug-pro.git
cd chronodebug-pro

## 💻 How to Use
Launch ChronoDebug Pro
Load your C++ program or integrate the profiling library
Run your program through the tool
View the visual timeline showing function execution
Analyze performance metrics using color indicators
Export reports for documentation or sharing

📊 Screenshots
Interface
<img width="899" height="537" alt="image" src="https://github.com/user-attachments/assets/c53af0b4-ca75-4c37-a63b-f6360eb34e2d" />

BubbleSort(SLOW)
<img width="898" height="537" alt="image" src="https://github.com/user-attachments/assets/c7fc12f8-faba-4046-85fd-94219b025a8d" />

BubbleSort(Medium/Slow)
<img width="900" height="537" alt="image" src="https://github.com/user-attachments/assets/57bb0e45-ecff-4dd0-97ed-a0d00903e666" />

BubbleSort(Fast)
<img width="897" height="540" alt="image" src="https://github.com/user-attachments/assets/bd5cc437-bc33-469c-9cbb-79e998ef552d" />

📁 Project Structure

chronodebug-pro/
├── main.cpp              # Main application entry point
├── mainwindow.cpp        # Main window implementation
├── mainwindow.h          # Main window header
├── mainwindow.ui         # Qt UI design file
├── profiler.cpp          # Profiling functionality
└── QtCreator/            # Qt project files


🎓 Academic Information
Project Type: Semester Project(BSCS)
Course: Fundamentals of Computer Programming
Year: 2025-26
Semester: 1st Semester
Institution: NUST(National University Of Sciences And Technology)
Team Size: Solo Project

🔮 Future Enhancements
Visualisation of Execution of Code
Memory usage profiling
Multi-threaded program support
Integration with popular IDEs as plugins
Cloud-based report sharing
Advanced filtering and search capabilities
Comparison mode for before/after optimization

📝 License
This project is created for educational purposes.

👨‍💻 Author
Sibghatullah

LinkedIn: www.linkedin.com/in/sibghatullah-saeed-1b00773b5
Email: ssibghatullah0@gmail.com
GitHub: @sibghatullah99

🙏 Acknowledgments
Thanks to course instructor for guidance
Inspired by professional profiling tools like Valgrind and gprof
Qt Framework documentation and community
