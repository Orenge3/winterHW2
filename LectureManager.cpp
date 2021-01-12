

#include "LectureManager.h"

StatusTypeCL LectureManager::AddCourse(int courseID) {
    try {
        Course * Course_to_add = new Course(courseID);
        return (StatusTypeCL)courses.Insert(courseID,Course_to_add);
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR_CL;
    }
}

StatusTypeCL LectureManager::RemoveCourse(int courseID) {
    Course* tempCourseInTable = courses.Find(courseID);

    //course does not exist
    if (!tempCourseInTable) return FAILURE_CL;

    int numOfLessons = tempCourseInTable->GetCourseNumOfLessons();
    //course has no Lessons
    if (numOfLessons == 0){
        //simply remove from hash table
        return (StatusTypeCL)courses.Remove(courseID);
    }
    totalNumOfLessons -= numOfLessons;
    //delete all Lessons with views from viewed tree,
    //and all lesson with no views from course itself
    for (int i = 0; i < numOfLessons; ++i) {
        HASH_TABLE<Lesson> * lessons = tempCourseInTable->GetLessonsTable();
        Lesson* tempLesson = lessons->Find(i);
        //lesson not found - not supposed to happen
        if (!tempLesson){
            //lesson has views
            if (tempLesson->GetTimeWatched() > 0){
                //TODO- how do ID lessons in tree?
                //TODO -currently using lesson ID
                //delete from tree and table
                viewedNumOfLessons--;
                viewed_lessons_tree.Delete(tempLesson->GetLessonID());
            }//delete only from table
            lessons->Remove(i);
            delete tempLesson;
        }
    }
    courses.Remove(courseID);
    delete tempCourseInTable;
    return SUCCESS_CL;
}

StatusTypeCL LectureManager::AddLecture(int courseID, int *lectureID) {
    Course* tempCourseInTable = courses.Find(courseID);

    //course does not exist
    if (!tempCourseInTable) return FAILURE_CL;
    int numOfLessons = tempCourseInTable->GetCourseNumOfLessons();
    try {
        Lesson *Lesson_to_add = new Lesson(courseID,numOfLessons);
        *lectureID = numOfLessons;
        tempCourseInTable->IncNumOfLessons();
        totalNumOfLessons++;
        return (StatusTypeCL)tempCourseInTable->
        GetLessonsTable()->Insert(*lectureID,Lesson_to_add);
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR_CL;
    }
}

StatusTypeCL LectureManager::WatchClass(int courseID, int classID, int time) {
    Course* tempCourseInTable = courses.Find(courseID);

    //course does not exist
    if (!tempCourseInTable) return FAILURE_CL;

    if (classID+1 > tempCourseInTable->GetCourseNumOfLessons()){
        return INVALID_INPUT_CL;
    }
    Lesson * addTo = tempCourseInTable->GetLessonsTable()->Find(classID);
    /******* lesson does not exist -  not supposed to happen  ******/
    if (!addTo) return FAILURE_CL;

    // TODO do we take out of hash table or not?
    if (addTo->GetTimeWatched() == 0){ //not in viewed tree yet
        addTo->watchLesson(time);
        //TODO how do we tell apart lessons in your tree?
        viewed_lessons_tree.Insert(addTo,addTo->GetLessonID());
        viewedNumOfLessons++;
        return SUCCESS_CL;
//        if(Not_Viewed_course_ptr->GetNotViewedList() == nullptr) {
//            if( this->not_viewed_courses.Delete(Not_Viewed_course_ptr) != AVL_SUCCESS) {
//                delete toFind;
//                delete Found_out_node;
//                return FAILURE_CL;
//            }
//        }
//        addTo->watchLesson(time);
//        addTo->SetPointerToCharts(nullptr);
//        delete (toFind);
//        delete Found_out_node;
//        return (StatusTypeCL) this->viewed_lessons_tree.Insert(addTo);
    }
    //TODO argument in tree and in table are the same (we only hold pointers to it)
//    //find in watched tree
//    Lesson* temp = *viewed_lessons_tree.Find(addTo->GetLessonID());
    viewed_lessons_tree.Delete(addTo->GetLessonID());
    addTo->watchLesson(time);
    //delete from tree and re insert
    viewed_lessons_tree.Insert(addTo,addTo->GetLessonID());
    return SUCCESS_CL;
}

StatusTypeCL
LectureManager::TimeViewed(int courseID, int classId, int *timeViewed) {
    Course * toFind = courses.Find(courseID);

    //course not found
    if (!toFind) return FAILURE_CL;
    //parameter check
    if (classId+1 > toFind->GetCourseNumOfLessons()){
        return INVALID_INPUT_CL;
    }

    HASH_TABLE<Lesson> * lessons = toFind->GetLessonsTable();
    Lesson * lessonToFind = lessons->Find(classId);
    //not supposed to happen
    if (!lessonToFind) return INVALID_INPUT_CL;
    *timeViewed = lessonToFind->GetTimeWatched();
    return SUCCESS_CL;

//    Course * toFind = new Course(courseID);
//    AVLTree<Course*>::node * found_node = new AVLTree<Course*>::node(); ////added allocation in all functions to avoid segfault
//    if (this->course_tree.Find(toFind, found_node) == AVL_FAILURE){
//        delete found_node;
//        delete toFind;
//        return FAILURE_CL;
//    }
//    if (classId+1 > (*found_node->_data)->GetCourseNumOfLessons()){
//        delete found_node;
//        delete toFind;
//        return INVALID_INPUT_CL;
//    } //// changed to class ID
//    *timeViewed = (*found_node->_data)->GetLessonArray()[classId].GetTimeWatched(); ////changed to found out node since date is not transferred out
//    delete found_node;
//    delete toFind;
//    return SUCCESS_CL;
}
//void FillArrayFromViewed(int** courses, int** classes, int* counter, int numOfClasses,  AVLTree<Lesson*>::node* root) {
//    if (root != nullptr)
//    {
//        FillArrayFromViewed(courses, classes, counter, numOfClasses, root->rightChild);
//        if (*counter != 0)
//        {
//            (*courses)[numOfClasses - *counter] = (*(*root->_data)).GetCourse();
//            //printf(" viewed into courses = %d \n", (*(*root->_data)).GetCourse());
//            (*classes)[numOfClasses - *counter] = (*(*root->_data)).GetLessonID();
//            //printf(" viewed into classes = %d \n", (*(*root->_data)).GetLessonID());
//            (*counter)--;
//        }
//        else
//            return;
//        FillArrayFromViewed(courses, classes, counter, numOfClasses, root->leftChild);
//    }
//}
//void FillArrayFromNotViewed(int** courses, int** classes, int* counter, int numOfClasses , AVLTree<Course*>::node* AVL_Node,
//                            AVLTree<Course*> * tree){
//    while (*counter != 0){
//        DLList<Lesson*> * List = (*(AVL_Node->_data))->GetNotViewedList(); //// ****** now problem starts here
//        Node<Lesson*> * list_node = List->GetHead();
//        while (list_node != nullptr){
//            (*courses)[numOfClasses- *counter] = (*list_node->_data)->GetCourse();
//            //printf("not viewed into courses place %d \n", numOfClasses-*counter);
//            (*classes)[numOfClasses- *counter] = (*list_node->_data)->GetLessonID();
//            //printf("not viewed into classes = %d \n", (*(*list_node->_data)).GetLessonID());
//            (*counter)--;
//            if (*counter == 0) return; //needed to check for finish in this while
//            //otherwise creates endless loop
//            list_node = list_node->next;
//        }
//        if(*counter != 0){
//            //// i think we unnecessarily deleted
////            Course * temp = new Course();
//            Course* temp = nullptr;
//            tree->treeGetNextInOrder(temp);
////            delete temp;
//            AVL_Node = tree->currentNode;
//        }
//    }
//}

StatusTypeCL LectureManager::GetIthWatchedClass(int i, int *courseID,
                                                int *classID){

    if (i > this->viewedNumOfLessons)
        return FAILURE_CL;
    AVLNode<Lesson*> *tempNode = viewed_lessons_tree.GetKHighestNde(i);
    *courseID = tempNode->data->GetCourse();
    *classID = tempNode->data->GetLessonID();
//    int counter = i;
//    FillArrayFromViewed(&courses, &classes, &counter, i, root);
//    // set current node to smallest
//    Course * data = nullptr;
//    this->not_viewed_courses.treeGetSmallest(data);
//    if (i >0) {
//        FillArrayFromNotViewed(&courses, &classes, &counter, i, this->not_viewed_courses.currentNode, &this->not_viewed_courses);
//    }
    return SUCCESS_CL;
}

void LectureManager::Quit() {
//    this->viewed_lessons_tree.DeleteTree(false);
//    this->course_tree.DeleteTree(true);
}
