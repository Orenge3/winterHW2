
#ifndef DS_HW2_WET_COURSE_H
#define DS_HW2_WET_COURSE_H
#include "DS_AVL.h"
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
    DLList<Lesson*> no_views_list;
    Lesson* LessonArray;

public:
    Course() = default;
    Course(int courseId, int numOfLessons = 0):
            CourseID(courseId), numOfLessons(numOfLessons), no_views_list(), LessonArray(nullptr){
        if (numOfLessons != 0){
            LessonArray = new Lesson[numOfLessons]();
        }
        if (numOfLessons == 0 ) {
            LessonArray = nullptr;
        }
        for (int i = 0; i < numOfLessons; ++i) {
            LessonArray[i] = Lesson(courseId,i);
            Lesson* LessonInArray = &LessonArray[i];
//            Lesson* LessonInArray = new Lesson(courseId, i);
            Node<Lesson*>* temp = no_views_list.GetLast();
            no_views_list.Insert(temp, LessonInArray);
            LessonInArray->SetPointerToCharts(no_views_list.GetLast());
            LessonArray[i] = *LessonInArray;
            //delete LessonInArray;/***/ fixed no allocation
        }
    };
    ~Course(){
        delete[] LessonArray;
    };
    int GetCourseID(){return CourseID;}
    int GetCourseNumOfLessons(){return numOfLessons;}
    DLList<Lesson*> * GetNotViewedList(){return &no_views_list;}
    Lesson* GetLessonArray(){return LessonArray;}
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
