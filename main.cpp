#include <iostream>
#include <chrono>
#include <random>
#include "AUList/AUList.h"
#include "ASList/ASList.h"
#include "BST/BST.h"
#include "LLSList/LLSList.h"
#include "LLUList/LLUList.h"
using namespace std;

typedef chrono::high_resolution_clock::time_point htimep;

template<typename L>
L CreateList() {
    L list;
    cout << "\t" << typeid(list).name() << ": ";
    list.PrintList();
    return list;
}

template<typename L>
chrono::duration<double> Insert(L &list, const int number, uniform_int_distribution<>& dis, mt19937& gen) {
    htimep start = chrono::high_resolution_clock::now();

    for ( int i = 0; i < number; i++ ) {
        int random_number = dis(gen);
        list.PutItem(random_number);
    }

    htimep end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "\t" << typeid(L).name() << ": " << fixed << elapsed.count() << " s" << endl;
    return elapsed;
}

template<typename L>
chrono::duration<double> Search(L &list, const int value) {
    htimep start = chrono::high_resolution_clock::now();

    list.GetItem(value);

    htimep end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "\t" << typeid(L).name() << ": " << fixed << elapsed.count() << " s" << endl;

    return elapsed;
}

template<typename L>
chrono::duration<double> Empty(L& list) {
    htimep start = chrono::high_resolution_clock::now();
    list.MakeEmpty();
    htimep end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "\t" << typeid(L).name() << ": " << fixed << elapsed.count() << " s" << endl;
    return elapsed;
}

template<typename L>
chrono::duration<double> Delete(L& list, const int value) {
    htimep start = chrono::high_resolution_clock::now();
    list.DeleteItem(value);
    htimep end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    return elapsed;
}

int main() {
    cout << "Performance Comparing" << endl;

    // Random number setup
    int num_items = 100;
    constexpr int loops = 100;
    constexpr int num_lists = 5;
    chrono::duration<double> search_times[loops][num_lists];
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, num_items);

    // Create structures
    cout << "Creating structures..." << endl;
    ASList test_as_list = CreateList<ASList>();
    AUList test_au_list = CreateList<AUList>();
    BST test_bst = CreateList<BST>();
    LLSList test_lls_list = CreateList<LLSList>();
    LLUList test_llu_list = CreateList<LLUList>();
    cout << endl;

    // Insert data
    chrono::duration<double> insert_times[2][loops][num_lists];
    for ( int i = 0; i < loops; i++) {
        cout << "Emptying..." << endl;
        insert_times[0][i][0] = Empty(test_as_list);
        insert_times[0][i][1] = Empty(test_au_list);
        insert_times[0][i][2] = Empty(test_bst);
        insert_times[0][i][3] = Empty(test_lls_list);
        insert_times[0][i][4] = Empty(test_llu_list);
        cout << "Inserting " << num_items << " items..." << endl;
        insert_times[1][i][0] = Insert(test_as_list, num_items, dis, gen);
        insert_times[1][i][1] = Insert(test_au_list, num_items, dis, gen);
        insert_times[1][i][2] = Insert(test_bst, num_items, dis, gen);
        insert_times[1][i][3] = Insert(test_lls_list, num_items, dis, gen);
        insert_times[1][i][4] = Insert(test_llu_list, num_items, dis, gen);
        cout << endl;
    }

    // Search data
    for ( int i = 0; i < loops; i++) {
        int random_number = dis(gen);
        cout << "Searching for " << random_number << endl;
        search_times[i][0] = Search(test_as_list, random_number);
        search_times[i][1] = Search(test_au_list, random_number);
        search_times[i][2] = Search(test_bst, random_number);
        search_times[i][3] = Search(test_lls_list, random_number);
        search_times[i][4] = Search(test_llu_list, random_number);
    }

    // Delete data
    chrono::duration<double> delete_times[loops][num_lists];
    int loops_done = 0;
    int failcount = 0;
    while (loops_done < loops) {
        if (failcount > (num_items)) break;
        int random_number = dis(gen);
        if (test_as_list.GetItem(random_number) != -1 &&
            test_au_list.GetItem(random_number) != -1 &&
            test_bst.GetItem(random_number) != -1 &&
            test_lls_list.GetItem(random_number) != -1 &&
            test_llu_list.GetItem(random_number) != -1) {
            cout << "Deleting " << random_number << endl;
            delete_times[loops_done][0] = Delete(test_as_list, random_number);
            delete_times[loops_done][1] = Delete(test_au_list, random_number);
            delete_times[loops_done][2] = Delete(test_bst, random_number);
            delete_times[loops_done][3] = Delete(test_lls_list, random_number);
            delete_times[loops_done][4] = Delete(test_llu_list, random_number);
            failcount=0;
            loops_done++;
        } else {
            failcount++;
        }
    }

    double avgs_empty[num_lists];
    double avgs_insert[num_lists];
    double avgs_search[num_lists];
    double avgs_delete[num_lists];
    for ( int i = 0; i < num_lists; i++) {
        double avg_empty = 0;
        double avg_insert = 0;
        double avg_search = 0;
        double avg_delete = 0;
        for ( int j = 0; j < loops; j++) {
            avg_empty += insert_times[0][j][i].count();
            avg_insert += insert_times[1][j][i].count();
            avg_search += search_times[j][i].count();
            avg_delete += delete_times[j][i].count();
        }
        avgs_empty[i] = avg_empty/loops;
        avgs_insert[i] = avg_insert/loops;
        avgs_search[i] = avg_search/loops;
        avgs_delete[i] = avg_delete/loops;
    }
    cout << endl;
    cout << "num_items " << num_items << endl;
    cout << "loops " << loops << endl;
    cout << "ASList Avgs " << avgs_empty[0] << ", " << avgs_insert[0] << ", " << avgs_search[0] << ", " << avgs_delete[0] << endl;
    cout << "AUList Avgs " << avgs_empty[1] << ", " << avgs_insert[1] << ", " << avgs_search[1] << ", " << avgs_delete[1] << endl;
    cout << "BST Avgs " << avgs_empty[2] << ", " << avgs_insert[2] << ", " << avgs_search[2] << ", " << avgs_delete[2] << endl;
    cout << "LLSList Avgs " << avgs_empty[3] << ", " << avgs_insert[3] << ", " << avgs_search[3] << ", " << avgs_delete[3] << endl;
    cout << "LLUList Avgs " << avgs_empty[4] << ", " << avgs_insert[4] << ", " << avgs_search[4] << ", " << avgs_delete[4] << endl;

    cout << endl;

    return 0;
}