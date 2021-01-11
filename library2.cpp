//
// Created by oreno on 04-Dec-20.
//

#include "library.h"
#include "LectureManager.h"


void *Init() {
    try {
        LectureManager *DS = new LectureManager(); //##TODO check memory alloc
        return (void*)DS;
    }
    catch (std::bad_alloc&){
        return nullptr;
    }
}

StatusType AddCourse(void *DS, int courseID, int numOfClasses) {
    if (courseID <= 0 || DS == nullptr || numOfClasses <= 0) {//parameter check
        return INVALID_INPUT;
    }
    return (StatusType)((LectureManager*)DS)->AddCourse(courseID, numOfClasses);
}

StatusType RemoveCourse(void *DS, int courseID) {
    if (DS == nullptr || courseID <= 0) return INVALID_INPUT;
    return (StatusType)((LectureManager*) DS)->RemoveCourse(courseID);
}

StatusType WatchClass(void *DS, int courseID, int classID, int time) {
    if (DS == nullptr || classID < 0 || courseID <= 0 || time <= 0) return INVALID_INPUT;
    return (StatusType) ((LectureManager *) DS)->WatchClass(courseID, classID,
                                                            time);
}

StatusType TimeViewed(void *DS, int courseID, int classID, int *timeViewed) {
    if (classID < 0 || DS == nullptr || courseID <= 0 || timeViewed == nullptr)
        return INVALID_INPUT;
    return (StatusType)((LectureManager*) DS)->TimeViewed(courseID, classID,
                                                          timeViewed);
}

StatusType
GetMostViewedClasses(void *DS, int numOfClasses, int *courses, int *classes) {
    if (DS == nullptr || numOfClasses <= 0) return INVALID_INPUT;
    return (StatusType)((LectureManager*)DS)->GetMostViewedClasses(numOfClasses,
                                                                   courses,
                                                                   classes);
}

void Quit(void **DS) {
    if (DS == nullptr || *DS == nullptr) return;
    ((LectureManager*)*DS)->Quit();
    delete (LectureManager*) *DS;
    *DS = nullptr;
}
