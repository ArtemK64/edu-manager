#ifndef GRADE_H
#define GRADE_H

#include <string>

class Grade {
private:
    int studentId;
    int courseId;
    double value;
    std::string gradeType;

public:
    Grade(int studentId, int courseId, double value, const std::string& gradeType);

    void setStudentId(int newStudentId);
    void setCourseId(int newCourseId);
    void setValue(double newValue);
    void setGradeType(const std::string& newGradeType);

    bool create();
    bool update();
    bool remove();

    void printInfo() const;
};

#endif