#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "wordlist.txt"
#define SONG "test/test1.txt"
#define ALLNUM "test/allnumtest.txt"
#define NUMNTEXT "test/numandtext.txt"


START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* incorrect_word = "Caoimhín";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(incorrect_word, hashtable));
}
END_TEST

START_TEST(test_check_word_buffer_overflow)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char incorrect_word[500000];
    for(int i = 0; i < 499999; i++)
        incorrect_word[i] = 'A';
    incorrect_word[499999] = 0;
    ck_assert(!check_word(incorrect_word, hashtable));
}
END_TEST

START_TEST(test_check_word_lower_case)
{
    char * misspelled[1000];
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    FILE* song;
    song = fopen(SONG, "r");
    int misspell = check_words(song, hashtable, misspelled);
    ck_assert_int_eq(misspell,14);
    fclose(song);
}
END_TEST

START_TEST(test_check_word_num)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* anum = "51545481521";
    ck_assert(check_word(anum, hashtable));
}
END_TEST

START_TEST(test_check_word_all_num)
{
    char * misspelled[1000];
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    FILE* allnum;
    allnum = fopen(ALLNUM, "r");
    int misspell = check_words(allnum, hashtable, misspelled);
    ck_assert_int_eq(misspell,0);
    fclose(allnum);
}
END_TEST

START_TEST(test_check_word_num_and_text)
{
    char * misspelled[1000];
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    FILE* numandtext;
    numandtext = fopen(NUMNTEXT, "r");
    int misspell = check_words(numandtext, hashtable, misspelled);
    ck_assert_int_eq(misspell,6);
    fclose(numandtext);
}
END_TEST

Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_word_buffer_overflow);
    tcase_add_test(check_word_case, test_check_word_lower_case);
    tcase_add_test(check_word_case, test_check_word_num);
    tcase_add_test(check_word_case, test_check_word_all_num);
    tcase_add_test(check_word_case, test_check_word_num_and_text);
    suite_add_tcase(suite, check_word_case);

    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

