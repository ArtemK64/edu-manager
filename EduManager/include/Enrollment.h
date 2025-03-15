#ifndef ENROLLMENT_H
#define ENROLLMENT_H

class Enrollment {
private:
    int studentId;
    int courseId;

public:
    Enrollment(int studentId, int courseId);

    int getStudentId() const;
    int getCourseId() const;

    void setStudentId(int newStudentId);
    void setCourseId(int newCourseId);

    bool create();
    bool update();
    bool remove();

    void printInfo() const;
};

#endif