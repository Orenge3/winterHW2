
/*              implementation of class can vary,
 *              but all definitions are needed to run main      */
#ifndef DS_HW2_WET_LECTUREMANAGER_H
#define DS_HW2_WET_LECTUREMANAGER_H

#include "DS_AVL.h" //we can change tree
#include "DLLIST.h" //we can change list

#include "library.h"
#include "Course.h"

typedef enum {
    SUCCESS_CL = 0,
    FAILURE_CL = -1,
    ALLOCATION_ERROR_CL = -2,
    INVALID_INPUT_CL = -3
} StatusTypeCL;


class LectureManager {
private:
    AVLTree<Course*> course_tree;
    AVLTree<Course*> not_viewed_courses;
    AVLTree<Lesson*> viewed_lessons_tree;
    int totalNumOfLessons;
public:
    LectureManager() = default;
    ~LectureManager() = default;
    StatusTypeCL AddCourse(int courseID, int numOfClasses);
    StatusTypeCL RemoveCourse(int courseID);
    StatusTypeCL WatchClass(int courseID, int classID, int time);
    StatusTypeCL TimeViewed(int courseID, int classId, int *timeViewed);
    StatusTypeCL GetMostViewedClasses(int numOfClasses, int* courses, int* classes);
    void Quit();
};


#endif //DS_HW2_WET_LECTUREMANAGER_H
