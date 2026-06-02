// src/profiler.cpp
#include "../include/profiler.h"
#include <thread>

Profiler& Profiler::instance() {
    static Profiler p;
    return p;
}

void Profiler::begin(const std::string& name, const std::string& file, int line) {
    FunctionRecord rec;
    rec.name = name;
    rec.file = file.substr(file.find_last_of("/\\") + 1);
    rec.line = line;
    rec.start = std::chrono::steady_clock::now();
    rec.depth = currentDepth++;
    m_records.push_back(rec);
}

void Profiler::end() {
    if (m_records.empty()) return;

    auto& rec = m_records.back();
    rec.end = std::chrono::steady_clock::now();
    rec.duration = std::chrono::duration_cast<std::chrono::microseconds>(rec.end - rec.start);
    currentDepth--;

    // Color logic: green < 1ms, yellow < 10ms, red >= 10ms
    if (rec.duration.count() < 1000)      rec.color = QColor(100, 255, 100);      // green
    else if (rec.duration.count() < 10000) rec.color = QColor(255, 255, 100);   // yellow
    else                                   rec.color = QColor(255, 100, 100);      // red
}
