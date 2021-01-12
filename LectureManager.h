
/*              implementation of class can vary,
 *              but all definitions are needed to run main      */
#ifndef DS_HW2_WET_LECTUREMANAGER_H
#define DS_HW2_WET_LECTUREMANAGER_H

#include "hash_table.h" //we can change tree
#include "AVL.h" //we can change list

#include "Course.h"

typedef enum {
    SUCCESS_CL = 0,
    FAILURE_CL = -1,
    ALLOCATION_ERROR_CL = -2,
    INVALID_INPUT_CL = -3
} StatusTypeCL;


class LectureManager {
private:
    /**           implement new DB                   **/
    HASH_TABLE<Course> courses;
    AVLTree<Lesson*> viewed_lessons_tree;
    int totalNumOfLessons;
public:
    LectureManager() = default;
    ~LectureManager() = default;
    StatusTypeCL AddCourse(int courseID);
    StatusTypeCL RemoveCourse(int courseID);
    StatusTypeCL AddLecture(int courseID, int* lectureID);
    StatusTypeCL WatchClass(int courseID, int classID, int time);
    StatusTypeCL TimeViewed(int courseID, int classId, int *timeViewed);
    StatusTypeCL GetIthWatchedClass(int numOfClasses, int* courseID, int* classeID);
    void Quit();
};


#endif //DS_HW2_WET_LECTUREMANAGER_H
