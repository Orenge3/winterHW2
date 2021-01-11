

#include "LectureManager.h"

StatusTypeCL LectureManager::AddCourse(int courseID) {
    try {
        Course *Course_to_add = new Course(courseID);
    }
/**                     implement new DB                     **/
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR_CL;
    }
}

StatusTypeCL LectureManager::RemoveCourse(int courseID) {
    Course* tempCourseInTree = new Course(courseID);
    AVLTree<Course*>::node* out = new AVLTree<Course*>::node();
    Course* tempCourseInNotViewed;
    AVLTree<Course*>::node* temp;
    AVL_RESULT  res = course_tree.Find(tempCourseInTree,out);
    switch (res) {
        case AVL_FAILURE: //Course does not exist
            delete tempCourseInTree;
            delete out;
            return FAILURE_CL;
        case AVL_SUCCESS: {
            Course* found = *(out->_data);
            int numOfLessons = found->GetCourseNumOfLessons();
            totalNumOfLessons -= numOfLessons;
            bool all_lessons_viewed = true;
            for (int i = 0; i < numOfLessons; ++i) {
                //delete lessons
//                void *place = tempCourseInTree->GetLessonArray()[i].GetPointerToCharts();
                Lesson *tempLesson = &found->GetLessonArray()[i];
                if (tempLesson->GetPointerToCharts() ==
                    nullptr) {// it has views
                    //delete lesson from viewed list
                    viewed_lessons_tree.Delete(tempLesson);
                    continue;
                }
//                //delete lesson in not viewed
//                found->GetNotViewedList()->
//                Delete((Node<Lesson*>*)tempLesson->GetPointerToCharts());
                all_lessons_viewed = false;
            }
            //delete course in not viewed tree if it exits
            if (!all_lessons_viewed)
                not_viewed_courses.Delete(tempCourseInTree);
            //delete course from tree
            course_tree.Delete(tempCourseInTree);
//            delete[] found->GetLessonArray();
            delete found;
            delete tempCourseInTree;
            delete out;
            return SUCCESS_CL;
        }
        default:
            break;
    }
    delete tempCourseInTree;
    delete out;
    return FAILURE_CL;
}

StatusTypeCL LectureManager::AddLecture(int courseID, int *lectureID) {
    return ALLOCATION_ERROR_CL;
}

StatusTypeCL LectureManager::WatchClass(int courseID, int classID, int time) {
    Course * toFind = new Course(courseID);
    AVLTree<Course*>::node * Found_out_node = new AVLTree<Course*>::node();
    if (this->course_tree.Find(toFind, Found_out_node) == AVL_FAILURE) {
        delete toFind;
        delete Found_out_node;
        return FAILURE_CL;
    }
    Course * Course_Found = *(Found_out_node->_data);
    /** STILL important **/
    if (classID+1 > Course_Found->GetCourseNumOfLessons()){
        delete toFind;
        delete Found_out_node;
        return INVALID_INPUT_CL;
    }
    Lesson * addTo = & Course_Found->GetLessonArray()[classID];
    if (addTo->GetTimeWatched() == 0){
        //oren changed the source of the pointer
        Node<Lesson*> * L_node = (Node<Lesson*>*)Course_Found->GetLessonArray()[classID].GetPointerToCharts();
        if (L_node == nullptr)
            return ALLOCATION_ERROR_CL;
        Course_Found->GetNotViewedList()->Delete(L_node);
        this->not_viewed_courses.Find(toFind, Found_out_node);
        Course * Not_Viewed_course_ptr = *(Found_out_node->_data);
        ////removed for double delete. since our tree has pointers we have the
        //// same data in both trees can change this.
//        Not_Viewed_course_ptr->GetNotViewedList()->Delete(L_node);
        if(Not_Viewed_course_ptr->GetNotViewedList() == nullptr) {
            if( this->not_viewed_courses.Delete(Not_Viewed_course_ptr) != AVL_SUCCESS) {
                delete toFind;
                delete Found_out_node;
                return FAILURE_CL;
            }
        }
        addTo->watchLesson(time);
        addTo->SetPointerToCharts(nullptr);
        delete (toFind);
        delete Found_out_node;
        return (StatusTypeCL) this->viewed_lessons_tree.Insert(addTo);
    }
    AVLTree<Lesson*>::node * addToViewed_node = new AVLTree<Lesson*>::node();
    if (this->viewed_lessons_tree.Find(addTo, addToViewed_node) == AVL_FAILURE){
        delete addToViewed_node;
        delete toFind;
        delete Found_out_node;
        return FAILURE_CL;
    }
    delete addToViewed_node;
    delete Found_out_node;
    delete (toFind);
    this->viewed_lessons_tree.Delete(addTo);
    addTo->watchLesson(time);
    return (StatusTypeCL)this->viewed_lessons_tree.Insert(addTo);
}

StatusTypeCL
LectureManager::TimeViewed(int courseID, int classId, int *timeViewed) {
    Course * toFind = new Course(courseID);
    AVLTree<Course*>::node * found_node = new AVLTree<Course*>::node(); ////added allocation in all functions to avoid segfault
    if (this->course_tree.Find(toFind, found_node) == AVL_FAILURE){
        delete found_node;
        delete toFind;
        return FAILURE_CL;
    }
    if (classId+1 > (*found_node->_data)->GetCourseNumOfLessons()){
        delete found_node;
        delete toFind;
        return INVALID_INPUT_CL;
    } //// changed to class ID
    *timeViewed = (*found_node->_data)->GetLessonArray()[classId].GetTimeWatched(); ////changed to found out node since date is not transferred out
    delete found_node;
    delete toFind;
    return SUCCESS_CL;
}
void FillArrayFromViewed(int** courses, int** classes, int* counter, int numOfClasses,  AVLTree<Lesson*>::node* root) {
    if (root != nullptr)
    {
        FillArrayFromViewed(courses, classes, counter, numOfClasses, root->rightChild);
        if (*counter != 0)
        {
            (*courses)[numOfClasses - *counter] = (*(*root->_data)).GetCourse();
            //printf(" viewed into courses = %d \n", (*(*root->_data)).GetCourse());
            (*classes)[numOfClasses - *counter] = (*(*root->_data)).GetLessonID();
            //printf(" viewed into classes = %d \n", (*(*root->_data)).GetLessonID());
            (*counter)--;
        }
        else
            return;
        FillArrayFromViewed(courses, classes, counter, numOfClasses, root->leftChild);
    }
}
void FillArrayFromNotViewed(int** courses, int** classes, int* counter, int numOfClasses , AVLTree<Course*>::node* AVL_Node,
                            AVLTree<Course*> * tree){
    while (*counter != 0){
        DLList<Lesson*> * List = (*(AVL_Node->_data))->GetNotViewedList(); //// ****** now problem starts here
        Node<Lesson*> * list_node = List->GetHead();
        while (list_node != nullptr){
            (*courses)[numOfClasses- *counter] = (*list_node->_data)->GetCourse();
            //printf("not viewed into courses place %d \n", numOfClasses-*counter);
            (*classes)[numOfClasses- *counter] = (*list_node->_data)->GetLessonID();
            //printf("not viewed into classes = %d \n", (*(*list_node->_data)).GetLessonID());
            (*counter)--;
            if (*counter == 0) return; //needed to check for finish in this while
            //otherwise creates endless loop
            list_node = list_node->next;
        }
        if(*counter != 0){
            //// i think we unnecessarily deleted
//            Course * temp = new Course();
            Course* temp = nullptr;
            tree->treeGetNextInOrder(temp);
//            delete temp;
            AVL_Node = tree->currentNode;
        }
    }
}

StatusTypeCL LectureManager::GetIthWatchedClass(int numOfClasses, int *courseID,
                                                int *classeID){
    if (numOfClasses > this->totalNumOfLessons)
        return FAILURE_CL;
    int counter = numOfClasses;
    FillArrayFromViewed(&courses, &classes, &counter, numOfClasses, root);
    // set current node to smallest
    Course * data = nullptr;
    this->not_viewed_courses.treeGetSmallest(data);
    if (numOfClasses >0) {
        FillArrayFromNotViewed(&courses, &classes, &counter, numOfClasses, this->not_viewed_courses.currentNode, &this->not_viewed_courses);
    }
    return SUCCESS_CL;
}

void LectureManager::Quit() {
//    this->viewed_lessons_tree.DeleteTree(false);
//    this->course_tree.DeleteTree(true);
}
