
#ifndef DS_HW2_WET_COURSE_H
#define DS_HW2_WET_COURSE_H
#include "AVL.h"
const int ZERO_STREAMS = 0;
class Lesson{
    int courseId;
    int lessonID;
    int timeViewed;
    void* pointer;
public:
    Lesson() = default;
    Lesson(int courseID, int lessonID): courseId(courseID), lessonID(lessonID)
            , timeViewed(ZERO_STREAMS), pointer(nullptr){}
    ~Lesson() = default;
    int GetCourse() const {return courseId;}
    int GetLessonID() const {return lessonID;}
    int GetTimeWatched() const {return timeViewed;}
    void watchLesson(int time){ timeViewed += time;}
    void SetPointerToCharts(void* address){ pointer = address;}
    void* GetPointerToCharts(){return pointer;}
    Lesson& operator=(const Lesson & other){
        if (this != &other)
        {
            courseId = other.courseId;
            lessonID = other.lessonID;
            timeViewed = other.timeViewed;
            pointer = other.pointer;
        }
        return *this;
    }
    bool operator>(Lesson const & L) const
    {
        if (this->timeViewed > L.timeViewed)
            return true;
        else if ((this->timeViewed == L.timeViewed) && (this->courseId < L.courseId))
            return true;
        else
            return (this->timeViewed == L.timeViewed) &&
                   (this->courseId == L.courseId) &&
                   (this->lessonID < L.lessonID);
    }
    bool operator==(Lesson const & L) const
    {
        return (this->timeViewed == L.timeViewed) &&
               (this->courseId == L.courseId) &&
               (this->lessonID == L.lessonID);
    }
};


class Course {
private:
    int CourseID;
    int numOfLessons;
    HASH_TABLE<Lesson> Lessons;

public:
    Course() = default;
    Course(int courseId, int numOfLessons = 0):
            CourseID(courseId), numOfLessons(numOfLessons), Lessons(){};
    ~Course() = default;
    int GetCourseID(){return CourseID;}
    int GetCourseNumOfLessons(){return numOfLessons;}
    void IncNumOfLessons(){ numOfLessons++;}
    HASH_TABLE<Lesson> * GetLessonsTable(){return &Lessons;}
    bool operator>(Course const & C) const
    {
        return this->CourseID > C.CourseID;
    }
    bool operator==(Course const & C) const
    {
        return this->CourseID == C.CourseID;
    }

};


#endif //DS_HW2_WET_COURSE_H
