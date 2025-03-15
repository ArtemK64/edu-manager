#ifndef STUDENTDEPARTMENT_H
#define STUDENTDEPARTMENT_H

class StudentDepartment {
private:
    int studentId;
    int departmentId;

public:
    StudentDepartment(int studentId, int departmentId);

    void setStudentId(int newStudentId);
    void setDepartmentId(int newDepartmentId);

    bool create();
    bool update();
    bool remove();

    void printInfo() const;
};

#endif