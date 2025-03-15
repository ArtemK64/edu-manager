#ifndef TEACHERDEPARTMENT_H
#define TEACHERDEPARTMENT_H

class TeacherDepartment {
private:
    int teacherId;
    int departmentId;

public:
    TeacherDepartment(int teacherId, int departmentId);

    void setTeacherId(int newTeacherId);
    void setDepartmentId(int newDepartmentId);

    bool create();
    bool update();
    bool remove();

    void printInfo() const;
};

#endif