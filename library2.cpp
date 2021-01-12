//
// Created by oreno on 04-Dec-20.
//

#include "library2.h"
#include "LectureManager.h"

/**          O(1) Worst            */
void *Init() {
    try {
        LectureManager *DS = new LectureManager(); //##TODO check memory alloc
        return (void*)DS;
    }
    catch (std::bad_alloc&){
        return nullptr;
    }
}
/**            O(1) input average            */
StatusType AddCourse(void *DS, int courseID) {
    if (courseID <= 0 || DS == nullptr) {//parameter check
        return INVALID_INPUT;
    }
    return (StatusType)((LectureManager*)DS)->AddCourse(courseID);
}

/**          O(m*log(M)) input average M-total lectures m -course lectures   */
StatusType RemoveCourse(void *DS, int courseID) {
    if (DS == nullptr || courseID <= 0) return INVALID_INPUT;
    return (StatusType)((LectureManager*) DS)->RemoveCourse(courseID);
}

/**            O(1) input average          */
StatusType AddClass(void *DS, int courseID, int *classID) {
    if (DS == nullptr || courseID<=0) return INVALID_INPUT;
    return (StatusType) ((LectureManager *) DS)->AddLesson(courseID, classID);
}


/**            O(log(M+2)) input average          */
StatusType WatchClass(void *DS, int courseID, int classID, int time) {
    if (DS == nullptr || classID < 0 || courseID <= 0 || time <= 0) return INVALID_INPUT;
    return (StatusType) ((LectureManager *) DS)->WatchClass(courseID, classID,
                                                            time);
}

/**            O(1) input average          */
StatusType TimeViewed(void *DS, int courseID, int classID, int *timeViewed) {
    if (classID < 0 || DS == nullptr || courseID <= 0 || timeViewed == nullptr)
        return INVALID_INPUT;
    return (StatusType)((LectureManager*) DS)->TimeViewed(courseID, classID,
                                                          timeViewed);
}


/**            O(log(M+2)) Worst          */
StatusType
GetIthWatchedClass(void *DS, int i, int *courseID, int *classID) {
    if (DS == nullptr || i <= 0) return INVALID_INPUT;
    return (StatusType)((LectureManager*)DS)->GetIthWatchedClass(i,
                                                                   courseID,
                                                                   classID);
}


/**            O(n + m) Worst          */
void Quit(void **DS) {
    if (DS == nullptr || *DS == nullptr) return;
    ((LectureManager*)*DS)->Quit();
    delete (LectureManager*) *DS;
    *DS = nullptr;
}
