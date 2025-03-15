#ifndef TEACHERCOURSE_H
#define TEACHERCOURSE_H

class TeacherCourse {
private:
    int teacherId;
    int courseId;

public:
    TeacherCourse(int teacherId, int courseId);

    void setTeacherId(int newTeacherId);
    void setCourseId(int newCourseId);

    bool create();
    bool update();
    bool remove();

    void printInfo() const;
};

#endif