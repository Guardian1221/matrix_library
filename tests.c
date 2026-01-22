#include <check.h>

#include "matrix.h"

///////////////tests-for-create_matrix
START_TEST(test_create_matrix_2x3_success) {
  matrix_t m;
  int error = s21_create_matrix(2, 3, &m);

  ck_assert_int_eq(error, OK);

  ck_assert_int_eq(m.rows, 2);
  ck_assert_int_eq(m.columns, 3);

  ck_assert_ptr_ne(m.matrix, NULL);

  for (int i = 0; i < m.rows; i++) {
    for (int j = 0; j < m.columns; j++) {
      ck_assert_double_eq(m.matrix[i][j], 0.0);
    }
  }

  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_create_matrix_zero_rows_return_error_nc) {
  matrix_t m;
  int error = s21_create_matrix(0, 5, &m);

  ck_assert_int_eq(error, ERROR_NC);
}
END_TEST

START_TEST(test_create_matrix_negative_1x3_return_error_nc) {
  matrix_t m;
  int error = s21_create_matrix(-1, 3, &m);

  ck_assert_int_eq(error, ERROR_NC);
}
END_TEST

START_TEST(test_create_matrix_2x2_null_ptr_return_error_nc) {
  int error = s21_create_matrix(2, 2, NULL);

  ck_assert_int_eq(error, ERROR_NC);
}
END_TEST

///////////////tests-for-remove
START_TEST(test_remove_valid_matrix) {
  matrix_t m;
  int error = s21_create_matrix(2, 2, &m);
  ck_assert_int_eq(error, OK);

  s21_remove_matrix(&m);

  ck_assert_ptr_eq(m.matrix, NULL);
  ck_assert_int_eq(m.rows, 0);
  ck_assert_int_eq(m.columns, 0);
}
END_TEST

START_TEST(test_remove_twice) {
  matrix_t m;
  s21_create_matrix(1, 1, &m);

  s21_remove_matrix(&m);
  s21_remove_matrix(&m);

  ck_assert_ptr_eq(m.matrix, NULL);
  ck_assert_int_eq(m.rows, 0);
  ck_assert_int_eq(m.columns, 0);
}
END_TEST

START_TEST(test_remove_null_pointer) { s21_remove_matrix(NULL); }
END_TEST

START_TEST(test_remove_broken_matrix) {
  matrix_t m;
  m.matrix = NULL;
  m.rows = 5;
  m.columns = 3;

  s21_remove_matrix(&m);

  ck_assert_ptr_eq(m.matrix, NULL);
  ck_assert_int_eq(m.rows, 0);
  ck_assert_int_eq(m.columns, 0);
}
END_TEST

///////////////tests-for-matrix-eq
START_TEST(test_matrix_eq_2x2_success) {
  matrix_t m1;
  matrix_t m2;

  s21_create_matrix(2, 2, &m1);
  s21_create_matrix(2, 2, &m2);

  m1.matrix[0][0] = 1.0;
  m1.matrix[0][1] = 2.0;
  m1.matrix[1][0] = 3.0;
  m1.matrix[1][1] = 4.0;

  m2.matrix[0][0] = 1.0;
  m2.matrix[0][1] = 2.0;
  m2.matrix[1][0] = 3.0;
  m2.matrix[1][1] = 4.0;

  int error = s21_eq_matrix(&m1, &m2);

  ck_assert_int_eq(error, SUCCESS);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_matrix_eq_2x2_different_value_return_failure) {
  matrix_t m1;
  matrix_t m2;

  s21_create_matrix(2, 2, &m1);
  s21_create_matrix(2, 2, &m2);

  m1.matrix[0][0] = 1.0;
  m1.matrix[0][1] = 2.0;
  m1.matrix[1][0] = 3.0;
  m1.matrix[1][1] = 4.0;

  m2.matrix[0][0] = 1.0;
  m2.matrix[0][1] = 2.0;
  m2.matrix[1][0] = 3.0;
  m2.matrix[1][1] = 4.000001;

  int error = s21_eq_matrix(&m1, &m2);

  ck_assert_int_eq(error, FAILURE);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_matrix_eq_different_size_return_failure) {
  matrix_t m1;
  matrix_t m2;

  s21_create_matrix(1, 2, &m1);
  s21_create_matrix(2, 2, &m2);

  m1.matrix[0][0] = 1.0;
  m1.matrix[0][1] = 2.0;

  m2.matrix[0][0] = 1.0;
  m2.matrix[0][1] = 2.0;
  m2.matrix[1][0] = 3.0;
  m2.matrix[1][1] = 4.0;

  int error = s21_eq_matrix(&m1, &m2);

  ck_assert_int_eq(error, FAILURE);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_matrix_eq_null_matrix_return_failure) {
  matrix_t m1;

  s21_create_matrix(2, 2, &m1);

  m1.matrix[0][0] = 1.0;
  m1.matrix[0][1] = 2.0;
  m1.matrix[1][0] = 3.0;
  m1.matrix[1][1] = 4.0;

  int error = s21_eq_matrix(&m1, NULL);

  ck_assert_int_eq(error, FAILURE);

  s21_remove_matrix(&m1);
}
END_TEST

START_TEST(test_matrix_eq_null_return_failure) {
  int error = s21_eq_matrix(NULL, NULL);
  ck_assert_int_eq(error, FAILURE);
}
END_TEST

START_TEST(test_matrix_eq_epsilon_success) {
  matrix_t m1;
  matrix_t m2;

  s21_create_matrix(2, 2, &m1);
  s21_create_matrix(2, 2, &m2);

  m1.matrix[0][0] = 1.0;
  m1.matrix[0][1] = 2.0;
  m1.matrix[1][0] = 3.0;
  m1.matrix[1][1] = 4.0;

  m2.matrix[0][0] = 1.0;
  m2.matrix[0][1] = 2.0 + 0.5 * EPSILON;
  m2.matrix[1][0] = 3.0;
  m2.matrix[1][1] = 4.0;

  int error = s21_eq_matrix(&m1, &m2);

  ck_assert_int_eq(error, SUCCESS);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

///////////////tests-for-matrix-sum
START_TEST(test_matrix_sum_2x2_success) {
  matrix_t m1;
  matrix_t m2;
  matrix_t result;
  matrix_t expected;

  s21_create_matrix(2, 2, &m1);
  s21_create_matrix(2, 2, &m2);
  s21_create_matrix(2, 2, &expected);

  m1.matrix[0][0] = 1.0;
  m1.matrix[0][1] = 2.0;
  m1.matrix[1][0] = 3.0;
  m1.matrix[1][1] = 4.0;

  m2.matrix[0][0] = 1.0;
  m2.matrix[0][1] = 2.0;
  m2.matrix[1][0] = 3.0;
  m2.matrix[1][1] = 4.0;

  expected.matrix[0][0] = 2.0;
  expected.matrix[0][1] = 4.0;
  expected.matrix[1][0] = 6.0;
  expected.matrix[1][1] = 8.0;

  int error = s21_sum_matrix(&m1, &m2, &result);

  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_matrix_sum_different_size_return_error_cl) {
  matrix_t m1;
  matrix_t m2;
  matrix_t result;

  s21_create_matrix(3, 2, &m1);
  s21_create_matrix(2, 2, &m2);

  m1.matrix[0][0] = 1.0;

  m2.matrix[0][0] = 3.0;

  int error = s21_sum_matrix(&m1, &m2, &result);

  ck_assert_int_eq(error, ERROR_CL);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_matrix_sum_null_first_return_error_nc) {
  matrix_t m2;
  matrix_t result;

  s21_create_matrix(2, 2, &m2);
  s21_create_matrix(2, 2, &result);

  m2.matrix[0][0] = 3.0;

  int error = s21_sum_matrix(NULL, &m2, &result);

  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&m2);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_matrix_sum_null_second_return_error_nc) {
  matrix_t m1;
  matrix_t result;

  s21_create_matrix(2, 2, &m1);
  s21_create_matrix(2, 2, &result);

  m1.matrix[0][0] = 3.0;

  int error = s21_sum_matrix(&m1, NULL, &result);

  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_matrix_sum_null_result_return_error_nc) {
  matrix_t m1;
  matrix_t m2;

  s21_create_matrix(2, 2, &m1);
  s21_create_matrix(2, 2, &m2);

  m1.matrix[0][0] = 2.0;
  m2.matrix[0][0] = 3.0;

  int error = s21_sum_matrix(&m1, &m2, NULL);

  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_matrix_sum_null_index_return_error_nc) {
  matrix_t A;
  A.rows = 2;
  A.columns = 2;
  A.matrix = calloc(2, sizeof(double *));

  A.matrix[0] = calloc(2, sizeof(double));
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;

  matrix_t B;
  s21_create_matrix(2, 2, &B);
  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0;

  matrix_t result;
  int error = s21_sum_matrix(&A, &B, &result);

  ck_assert_int_eq(error, ERROR_NC);

  free(A.matrix[0]);
  free(A.matrix);
  s21_remove_matrix(&B);
}
END_TEST

///////////////tests-for-matrix-sub
START_TEST(test_matrix_sub_2x2_success) {
  matrix_t m1;
  matrix_t m2;
  matrix_t result;
  matrix_t expected;

  s21_create_matrix(2, 2, &m1);
  s21_create_matrix(2, 2, &m2);
  s21_create_matrix(2, 2, &expected);

  m1.matrix[0][0] = 5.0;
  m1.matrix[0][1] = 6.0;
  m1.matrix[1][0] = 7.0;
  m1.matrix[1][1] = 8.0;

  m2.matrix[0][0] = 1.0;
  m2.matrix[0][1] = 2.0;
  m2.matrix[1][0] = 3.0;
  m2.matrix[1][1] = 4.0;

  expected.matrix[0][0] = 4.0;
  expected.matrix[0][1] = 4.0;
  expected.matrix[1][0] = 4.0;
  expected.matrix[1][1] = 4.0;

  int error = s21_sub_matrix(&m1, &m2, &result);

  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_matrix_sub_different_size_return_error_cl) {
  matrix_t m1;
  matrix_t m2;
  matrix_t result;

  s21_create_matrix(3, 2, &m1);
  s21_create_matrix(2, 2, &m2);

  m1.matrix[0][0] = 1.0;

  m2.matrix[0][0] = 3.0;

  int error = s21_sub_matrix(&m1, &m2, &result);

  ck_assert_int_eq(error, ERROR_CL);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_matrix_sub_null_first_return_error_nc) {
  matrix_t m2;
  matrix_t result;

  s21_create_matrix(2, 2, &m2);
  s21_create_matrix(2, 2, &result);

  m2.matrix[0][0] = 3.0;

  int error = s21_sub_matrix(NULL, &m2, &result);

  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&m2);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_matrix_sub_null_second_return_error_nc) {
  matrix_t m1;
  matrix_t result;

  s21_create_matrix(2, 2, &m1);
  s21_create_matrix(2, 2, &result);

  m1.matrix[0][0] = 3.0;

  int error = s21_sub_matrix(&m1, NULL, &result);

  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_matrix_sub_null_result_return_error_nc) {
  matrix_t m1;
  matrix_t m2;

  s21_create_matrix(2, 2, &m1);
  s21_create_matrix(2, 2, &m2);

  m1.matrix[0][0] = 2.0;
  m2.matrix[0][0] = 3.0;

  int error = s21_sub_matrix(&m1, &m2, NULL);

  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
}
END_TEST

START_TEST(test_matrix_sub_null_index_return_error_nc) {
  matrix_t A;
  A.rows = 2;
  A.columns = 2;
  A.matrix = calloc(2, sizeof(double *));

  A.matrix[0] = calloc(2, sizeof(double));
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;

  matrix_t B;
  s21_create_matrix(2, 2, &B);
  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0;

  matrix_t result;
  int error = s21_sub_matrix(&A, &B, &result);

  ck_assert_int_eq(error, ERROR_NC);

  free(A.matrix[0]);
  free(A.matrix);
  s21_remove_matrix(&B);
}
END_TEST

///////////////tests-for-matrix-mult-matrix
START_TEST(test_mult_matrix_2x3_3x2_success) {
  matrix_t A, B, result, expected;
  int err;

  err = s21_create_matrix(2, 3, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;

  err = s21_create_matrix(3, 2, &B);
  ck_assert_int_eq(err, OK);
  B.matrix[0][0] = 7;
  B.matrix[0][1] = 8;
  B.matrix[1][0] = 9;
  B.matrix[1][1] = 10;
  B.matrix[2][0] = 11;
  B.matrix[2][1] = 12;

  err = s21_create_matrix(2, 2, &expected);
  ck_assert_int_eq(err, OK);
  expected.matrix[0][0] = 58;
  expected.matrix[0][1] = 64;
  expected.matrix[1][0] = 139;
  expected.matrix[1][1] = 154;

  int error = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_matrix_incompatible_sizes_return_error_cl) {
  matrix_t A, B;
  int err;

  err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);

  err = s21_create_matrix(3, 3, &B);
  ck_assert_int_eq(err, OK);

  matrix_t result;
  int error = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(error, ERROR_CL);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_matrix_null_A_return_error_nc) {
  matrix_t B, result;
  int err = s21_create_matrix(2, 2, &B);
  ck_assert_int_eq(err, OK);
  B.matrix[0][0] = 1.0;

  int error = s21_mult_matrix(NULL, &B, &result);
  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_matrix_null_B_return_error_nc) {
  matrix_t A, result;
  int err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1.0;

  int error = s21_mult_matrix(&A, NULL, &result);
  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_mult_matrix_null_result_return_error_nc) {
  matrix_t A, B;
  int err;

  err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1.0;

  err = s21_create_matrix(2, 2, &B);
  ck_assert_int_eq(err, OK);
  B.matrix[0][0] = 2.0;

  int error = s21_mult_matrix(&A, &B, NULL);
  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_matrix_broken_A_return_error_nc) {
  matrix_t A;
  A.rows = 2;
  A.columns = 2;
  A.matrix = calloc(2, sizeof(double *));

  matrix_t B, result;
  int err = s21_create_matrix(2, 2, &B);
  ck_assert_int_eq(err, OK);
  B.matrix[0][0] = 1.0;

  int error = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(error, ERROR_NC);

  free(A.matrix);
  s21_remove_matrix(&B);
}
END_TEST

///////////////tests-for-matrix-mult-number
START_TEST(test_mult_number_2x2_success) {
  matrix_t A, result, expected;
  int err;

  err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  err = s21_create_matrix(2, 2, &expected);
  ck_assert_int_eq(err, OK);
  expected.matrix[0][0] = 2.0;
  expected.matrix[0][1] = 4.0;
  expected.matrix[1][0] = 6.0;
  expected.matrix[1][1] = 8.0;

  int error = s21_mult_number(&A, 2.0, &result);
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_number_null_A_return_error_nc) {
  matrix_t result;
  int error = s21_mult_number(NULL, 2.0, &result);
  ck_assert_int_eq(error, ERROR_NC);
}
END_TEST

START_TEST(test_mult_number_null_result_return_error_nc) {
  matrix_t A;
  int err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1.0;

  int error = s21_mult_number(&A, 2.0, NULL);
  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_mult_number_broken_matrix_return_error_nc) {
  matrix_t A;
  A.rows = 2;
  A.columns = 2;
  A.matrix = calloc(2, sizeof(double *));

  matrix_t result;
  int error = s21_mult_number(&A, 2.0, &result);
  ck_assert_int_eq(error, ERROR_NC);

  free(A.matrix);
}
END_TEST

START_TEST(test_mult_number_by_zero_success) {
  matrix_t A, result, expected;
  int err;

  err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = -5.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  err = s21_create_matrix(2, 2, &expected);
  ck_assert_int_eq(err, OK);

  int error = s21_mult_number(&A, 0.0, &result);
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

///////////////tests-for-matrix-transpose
START_TEST(test_transpose_2x3_success) {
  matrix_t A, result, expected;
  int err;

  err = s21_create_matrix(2, 3, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;

  err = s21_create_matrix(3, 2, &expected);
  ck_assert_int_eq(err, OK);
  expected.matrix[0][0] = 1;
  expected.matrix[0][1] = 4;
  expected.matrix[1][0] = 2;
  expected.matrix[1][1] = 5;
  expected.matrix[2][0] = 3;
  expected.matrix[2][1] = 6;

  int error = s21_transpose(&A, &result);
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_transpose_3x2_success) {
  matrix_t A, result, expected;
  int err;

  err = s21_create_matrix(3, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 4;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 5;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 6;

  err = s21_create_matrix(2, 3, &expected);
  ck_assert_int_eq(err, OK);
  expected.matrix[0][0] = 1;
  expected.matrix[0][1] = 2;
  expected.matrix[0][2] = 3;
  expected.matrix[1][0] = 4;
  expected.matrix[1][1] = 5;
  expected.matrix[1][2] = 6;

  int error = s21_transpose(&A, &result);
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_transpose_2x2_non_symmetric_success) {
  matrix_t A, result, expected;
  int err;

  err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  err = s21_create_matrix(2, 2, &expected);
  ck_assert_int_eq(err, OK);
  expected.matrix[0][0] = 1;
  expected.matrix[0][1] = 3;
  expected.matrix[1][0] = 2;
  expected.matrix[1][1] = 4;

  int error = s21_transpose(&A, &result);
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_transpose_2x2_symmetric_success) {
  matrix_t A, result;
  int err;

  err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 3;

  int error = s21_transpose(&A, &result);
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&A, &result), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_transpose_null_A_return_error_nc) {
  matrix_t result;
  int error = s21_transpose(NULL, &result);
  ck_assert_int_eq(error, ERROR_NC);
}
END_TEST

START_TEST(test_transpose_null_result_return_error_nc) {
  matrix_t A;
  int err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1.0;

  int error = s21_transpose(&A, NULL);
  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_transpose_broken_matrix_return_error_nc) {
  matrix_t A;
  A.rows = 2;
  A.columns = 2;
  A.matrix = calloc(2, sizeof(double *));

  matrix_t result;
  int error = s21_transpose(&A, &result);
  ck_assert_int_eq(error, ERROR_NC);

  free(A.matrix);
}
END_TEST

///////////////tests-for-matrix-determinant
START_TEST(test_determinant_1x1_success) {
  matrix_t A;
  int err = s21_create_matrix(1, 1, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 5.0;

  double det = 0.0;
  int error = s21_determinant(&A, &det);
  ck_assert_int_eq(error, OK);
  ck_assert_double_eq(det, 5.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_2x2_success) {
  matrix_t A;
  int err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  double det = 0.0;
  int error = s21_determinant(&A, &det);
  ck_assert_int_eq(error, OK);
  ck_assert_double_eq(det, -2.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_3x3_identity_success) {
  matrix_t A;
  int err = s21_create_matrix(3, 3, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;
  A.matrix[1][1] = 1;
  A.matrix[2][2] = 1;

  double det = 0.0;
  int error = s21_determinant(&A, &det);
  ck_assert_int_eq(error, OK);
  ck_assert_double_eq(det, 1.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_singular_matrix_success) {
  matrix_t A;
  int err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 4;

  double det = 0.0;
  int error = s21_determinant(&A, &det);
  ck_assert_int_eq(error, OK);
  ck_assert_double_eq(det, 0.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_null_A_return_error_nc) {
  double det;
  int error = s21_determinant(NULL, &det);
  ck_assert_int_eq(error, ERROR_NC);
}
END_TEST

START_TEST(test_determinant_null_result_return_error_nc) {
  matrix_t A;
  int err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;

  int error = s21_determinant(&A, NULL);
  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_non_square_return_error_nc) {
  matrix_t A;
  int err = s21_create_matrix(2, 3, &A);
  ck_assert_int_eq(err, OK);

  double det;
  int error = s21_determinant(&A, &det);
  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&A);
}
END_TEST

///////////////tests-for-matrix-calc-complements
START_TEST(test_calc_complements_2x2_success) {
  matrix_t A;
  int err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  matrix_t expected;
  err = s21_create_matrix(2, 2, &expected);
  ck_assert_int_eq(err, OK);
  expected.matrix[0][0] = 4;
  expected.matrix[0][1] = -3;
  expected.matrix[1][0] = -2;
  expected.matrix[1][1] = 1;

  matrix_t result;
  int error = s21_calc_complements(&A, &result);
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_calc_complements_1x1_success) {
  matrix_t A;
  int err = s21_create_matrix(1, 1, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 5.0;

  matrix_t expected;
  err = s21_create_matrix(1, 1, &expected);
  ck_assert_int_eq(err, OK);
  expected.matrix[0][0] = 1.0;

  matrix_t result;
  int error = s21_calc_complements(&A, &result);
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_calc_complements_null_A_return_error_nc) {
  matrix_t result;
  int error = s21_calc_complements(NULL, &result);
  ck_assert_int_eq(error, ERROR_NC);
}
END_TEST

START_TEST(test_calc_complements_null_result_return_error_nc) {
  matrix_t A;
  int err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;

  int error = s21_calc_complements(&A, NULL);
  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_non_square_return_error_nc) {
  matrix_t A;
  int err = s21_create_matrix(2, 3, &A);
  ck_assert_int_eq(err, OK);

  matrix_t result;
  int error = s21_calc_complements(&A, &result);
  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&A);
}
END_TEST

///////////////tests-for-inverse-matrix
START_TEST(test_inverse_1x1_success) {
  matrix_t A;
  int err = s21_create_matrix(1, 1, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 5.0;

  matrix_t expected;
  err = s21_create_matrix(1, 1, &expected);
  ck_assert_int_eq(err, OK);
  expected.matrix[0][0] = 1.0 / 5.0;

  matrix_t result;
  int error = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_inverse_2x2_success) {
  matrix_t A;
  int err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 4;
  A.matrix[0][1] = 7;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 6;

  matrix_t expected;
  err = s21_create_matrix(2, 2, &expected);
  ck_assert_int_eq(err, OK);
  expected.matrix[0][0] = 0.6;
  expected.matrix[0][1] = -0.7;
  expected.matrix[1][0] = -0.2;
  expected.matrix[1][1] = 0.4;

  matrix_t result;
  int error = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_inverse_3x3_identity_success) {
  matrix_t A;
  int err = s21_create_matrix(3, 3, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;
  A.matrix[1][1] = 1;
  A.matrix[2][2] = 1;

  matrix_t result;
  int error = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(s21_eq_matrix(&A, &result), SUCCESS);  // A == A^(-1)

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_inverse_singular_matrix_return_error_cl) {
  matrix_t A;
  int err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 4;

  matrix_t result;
  int error = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(error, ERROR_CL);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_non_square_return_error_nc) {
  matrix_t A;
  int err = s21_create_matrix(2, 3, &A);
  ck_assert_int_eq(err, OK);

  matrix_t result;
  int error = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_null_A_return_error_nc) {
  matrix_t result;
  int error = s21_inverse_matrix(NULL, &result);
  ck_assert_int_eq(error, ERROR_NC);
}
END_TEST

START_TEST(test_inverse_null_result_return_error_nc) {
  matrix_t A;
  int err = s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(err, OK);
  A.matrix[0][0] = 1;

  int error = s21_inverse_matrix(&A, NULL);
  ck_assert_int_eq(error, ERROR_NC);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_broken_matrix_return_error_nc) {
  matrix_t A;
  A.rows = 2;
  A.columns = 2;
  A.matrix = calloc(2, sizeof(double *));

  matrix_t result;
  int error = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(error, ERROR_NC);

  free(A.matrix);
}
END_TEST

Suite *matrix_suite(void) {
  Suite *s = suite_create("Matrix");
  TCase *tc_create = tcase_create("Creation");
  TCase *tc_remove = tcase_create("Removal");
  TCase *tc_eq = tcase_create("Equal");
  TCase *tc_sum = tcase_create("Sum");
  TCase *tc_sub = tcase_create("Sub");
  TCase *tc_mult_matrix = tcase_create("MultMatrix");
  TCase *tc_mult_number = tcase_create("MultNumber");
  TCase *tc_transpose = tcase_create("Transpose");
  TCase *tc_determinant = tcase_create("Determinant");
  TCase *tc_calc_complements = tcase_create("CalcComplements");
  TCase *tc_inverse = tcase_create("Inverse");

  tcase_add_test(tc_create, test_create_matrix_2x3_success);
  tcase_add_test(tc_create, test_create_matrix_zero_rows_return_error_nc);
  tcase_add_test(tc_create, test_create_matrix_negative_1x3_return_error_nc);
  tcase_add_test(tc_create, test_create_matrix_2x2_null_ptr_return_error_nc);

  tcase_add_test(tc_remove, test_remove_valid_matrix);
  tcase_add_test(tc_remove, test_remove_twice);
  tcase_add_test(tc_remove, test_remove_null_pointer);
  tcase_add_test(tc_remove, test_remove_broken_matrix);

  tcase_add_test(tc_eq, test_matrix_eq_2x2_success);
  tcase_add_test(tc_eq, test_matrix_eq_2x2_different_value_return_failure);
  tcase_add_test(tc_eq, test_matrix_eq_different_size_return_failure);
  tcase_add_test(tc_eq, test_matrix_eq_null_matrix_return_failure);
  tcase_add_test(tc_eq, test_matrix_eq_null_return_failure);
  tcase_add_test(tc_eq, test_matrix_eq_epsilon_success);

  tcase_add_test(tc_sum, test_matrix_sum_2x2_success);
  tcase_add_test(tc_sum, test_matrix_sum_different_size_return_error_cl);
  tcase_add_test(tc_sum, test_matrix_sum_null_first_return_error_nc);
  tcase_add_test(tc_sum, test_matrix_sum_null_second_return_error_nc);
  tcase_add_test(tc_sum, test_matrix_sum_null_result_return_error_nc);
  tcase_add_test(tc_sum, test_matrix_sum_null_index_return_error_nc);

  tcase_add_test(tc_sub, test_matrix_sub_2x2_success);
  tcase_add_test(tc_sub, test_matrix_sub_different_size_return_error_cl);
  tcase_add_test(tc_sub, test_matrix_sub_null_first_return_error_nc);
  tcase_add_test(tc_sub, test_matrix_sub_null_second_return_error_nc);
  tcase_add_test(tc_sub, test_matrix_sub_null_result_return_error_nc);
  tcase_add_test(tc_sub, test_matrix_sub_null_index_return_error_nc);

  tcase_add_test(tc_mult_matrix, test_mult_matrix_2x3_3x2_success);
  tcase_add_test(tc_mult_matrix,
                 test_mult_matrix_incompatible_sizes_return_error_cl);
  tcase_add_test(tc_mult_matrix, test_mult_matrix_null_A_return_error_nc);
  tcase_add_test(tc_mult_matrix, test_mult_matrix_null_B_return_error_nc);
  tcase_add_test(tc_mult_matrix, test_mult_matrix_null_result_return_error_nc);
  tcase_add_test(tc_mult_matrix, test_mult_matrix_broken_A_return_error_nc);

  tcase_add_test(tc_mult_number, test_mult_number_2x2_success);
  tcase_add_test(tc_mult_number, test_mult_number_null_A_return_error_nc);
  tcase_add_test(tc_mult_number, test_mult_number_null_result_return_error_nc);
  tcase_add_test(tc_mult_number,
                 test_mult_number_broken_matrix_return_error_nc);
  tcase_add_test(tc_mult_number, test_mult_number_by_zero_success);

  tcase_add_test(tc_transpose, test_transpose_2x3_success);
  tcase_add_test(tc_transpose, test_transpose_3x2_success);
  tcase_add_test(tc_transpose, test_transpose_2x2_non_symmetric_success);
  tcase_add_test(tc_transpose, test_transpose_2x2_symmetric_success);
  tcase_add_test(tc_transpose, test_transpose_null_A_return_error_nc);
  tcase_add_test(tc_transpose, test_transpose_null_result_return_error_nc);
  tcase_add_test(tc_transpose, test_transpose_broken_matrix_return_error_nc);

  tcase_add_test(tc_determinant, test_determinant_1x1_success);
  tcase_add_test(tc_determinant, test_determinant_2x2_success);
  tcase_add_test(tc_determinant, test_determinant_3x3_identity_success);
  tcase_add_test(tc_determinant, test_determinant_singular_matrix_success);
  tcase_add_test(tc_determinant, test_determinant_null_A_return_error_nc);
  tcase_add_test(tc_determinant, test_determinant_null_result_return_error_nc);
  tcase_add_test(tc_determinant, test_determinant_non_square_return_error_nc);

  tcase_add_test(tc_calc_complements, test_calc_complements_2x2_success);
  tcase_add_test(tc_calc_complements, test_calc_complements_1x1_success);
  tcase_add_test(tc_calc_complements,
                 test_calc_complements_null_A_return_error_nc);
  tcase_add_test(tc_calc_complements,
                 test_calc_complements_null_result_return_error_nc);
  tcase_add_test(tc_calc_complements,
                 test_calc_complements_non_square_return_error_nc);

  tcase_add_test(tc_inverse, test_inverse_1x1_success);
  tcase_add_test(tc_inverse, test_inverse_2x2_success);
  tcase_add_test(tc_inverse, test_inverse_3x3_identity_success);
  tcase_add_test(tc_inverse, test_inverse_singular_matrix_return_error_cl);
  tcase_add_test(tc_inverse, test_inverse_non_square_return_error_nc);
  tcase_add_test(tc_inverse, test_inverse_null_A_return_error_nc);
  tcase_add_test(tc_inverse, test_inverse_null_result_return_error_nc);
  tcase_add_test(tc_inverse, test_inverse_broken_matrix_return_error_nc);

  suite_add_tcase(s, tc_create);
  suite_add_tcase(s, tc_remove);
  suite_add_tcase(s, tc_eq);
  suite_add_tcase(s, tc_sum);
  suite_add_tcase(s, tc_sub);
  suite_add_tcase(s, tc_mult_matrix);
  suite_add_tcase(s, tc_mult_number);
  suite_add_tcase(s, tc_transpose);
  suite_add_tcase(s, tc_determinant);
  suite_add_tcase(s, tc_calc_complements);
  suite_add_tcase(s, tc_inverse);
  return s;
}

int main(void) {
  Suite *s;
  SRunner *sr;

  s = matrix_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}