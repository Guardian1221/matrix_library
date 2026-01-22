#include "matrix.h"

void print_matrix(matrix_t *A) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      printf("%lf ", A->matrix[i][j]);
    }
    printf("\n");
  }
}

bool square_matrix(matrix_t *A) { return A->rows == A->columns; }

bool arguments_check(matrix_t *A) {
  bool flag =
      !(A == NULL || A->matrix == NULL || A->columns < 1 || A->rows < 1);
  for (int i = 0; flag == true && i < A->rows; i++) {
    if (A->matrix[i] == NULL) {
      flag = false;
    }
  }
  return flag;
}

bool matrix_size(matrix_t *A, matrix_t *B) {
  bool flag = (A->columns == B->columns && A->rows == B->rows);
  return flag;
}

int create_matrix(int rows, int columns, matrix_t *result) {
  int error = OK;
  if (result == NULL) {
    error = ERROR_NC;
  } else if (rows < 1 || columns < 1) {
    error = ERROR_NC;
  }
  if (error == OK) {
    result->rows = rows;
    result->columns = columns;
    result->matrix = (double **)calloc(rows, sizeof(double *));
    if (result->matrix == NULL) {
      error = ERROR_CL;
    } else {
      int i = 0;
      for (i = 0; error == OK && i < rows; i++) {
        result->matrix[i] = (double *)calloc(columns, sizeof(double));
        if (result->matrix[i] == NULL) {
          error = ERROR_CL;
        }
      }
      if (error != OK) {
        for (int j = 0; j < i; j++) {
          free(result->matrix[j]);
        }
        free(result->matrix);
        result->matrix = NULL;
        result->rows = 0;
        result->columns = 0;
      }
    }
  }
  return error;
}

void remove_matrix(matrix_t *A) {
  if (A == NULL) {
    return;
  }
  if (A->matrix != NULL) {
    for (int n = 0; n < A->rows; n++) {
      free(A->matrix[n]);
    }
    free(A->matrix);
  }
  A->matrix = NULL;
  A->rows = 0;
  A->columns = 0;
}

int eq_matrix(matrix_t *A, matrix_t *B) {
  bool res = (arguments_check(A) && arguments_check(B)) && matrix_size(A, B);
  for (int i = 0; res == SUCCESS && i < A->rows; i++) {
    for (int j = 0; res == SUCCESS && j < A->columns; j++) {
      if (!(fabs(A->matrix[i][j] - B->matrix[i][j]) < EPSILON)) {
        res = FAILURE;
      }
    }
  }
  return res;
}

int sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = OK;
  if (!(arguments_check(A) && arguments_check(B)) || result == NULL) {
    error = ERROR_NC;
  }
  if (error == OK && !(matrix_size(A, B))) {
    error = ERROR_CL;
  }
  if (error == 0) {
    error = create_matrix(A->rows, A->columns, result);
    for (int i = 0; error == 0 && i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }
  return error;
}

int sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = OK;
  if (!(arguments_check(A) && arguments_check(B)) || result == NULL) {
    error = ERROR_NC;
  }
  if (error == OK && !(matrix_size(A, B))) {
    error = ERROR_CL;
  }
  if (error == 0) {
    error = create_matrix(A->rows, A->columns, result);
    for (int i = 0; error == 0 && i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }
  return error;
}

int mult_number(matrix_t *A, double number, matrix_t *result) {
  int error = OK;
  if ((!arguments_check(A)) || result == NULL) {
    error = ERROR_NC;
  }
  if (error == 0) {
    error = create_matrix(A->rows, A->columns, result);
    for (int i = 0; error == OK && i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }
  return error;
}

int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = OK;
  if (!arguments_check(A) || !arguments_check(B) || result == NULL) {
    error = ERROR_NC;
  }
  if (error == OK && A->columns != B->rows) {
    error = ERROR_CL;
  }
  if (error == OK) {
    error = create_matrix(A->rows, B->columns, result);
    for (int i = 0; error == OK && i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        for (int k = 0; k < A->columns; k++) {
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
  }
  return error;
}

int transpose(matrix_t *A, matrix_t *result) {
  int error = OK;
  if (!arguments_check(A) || result == NULL) {
    error = ERROR_NC;
  } else {
    error = create_matrix(A->columns, A->rows, result);
  }
  if (error == 0) {
    for (int i = 0; error == OK && i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }
  return error;
}

int calc_complements(matrix_t *A, matrix_t *result) {
  int error = OK;

  if (error == OK) {
    if (!arguments_check(A) || result == NULL || A->rows != A->columns) {
      error = ERROR_NC;
    }
  }
  if (error == OK && A->rows == 1) {
    error = create_matrix(1, 1, result);
    if (error == OK) {
      result->matrix[0][0] = 1.0;
    }
  }
  if (error == OK && A->rows > 1) {
    error = create_matrix(A->rows, A->columns, result);
    if (error == OK) {
      for (int i = 0; i < A->rows && error == OK; i++) {
        for (int j = 0; j < A->columns && error == OK; j++) {
          matrix_t minor;
          error = create_matrix(A->rows - 1, A->columns - 1, &minor);
          if (error == OK) {
            int mi = 0;
            for (int m = 0; m < A->rows; m++) {
              if (m != i) {
                int mj = 0;
                for (int n = 0; n < A->columns; n++) {
                  if (n != j) {
                    minor.matrix[mi][mj] = A->matrix[m][n];
                    mj++;
                  }
                }
                mi++;
              }
            }

            double det_minor = 0.0;
            int det_error = determinant(&minor, &det_minor);
            remove_matrix(&minor);

            if (det_error == OK) {
              result->matrix[i][j] = (pow(-1, i + j)) * det_minor;
            } else {
              error = det_error;
            }
          }
        }
      }
      if (error != OK) {
        remove_matrix(result);
      }
    }
  }
  return error;
}

int determinant(matrix_t *A, double *result) {
  int error = OK;
  if (error == OK) {
    if (!arguments_check(A) || result == NULL || A->rows != A->columns) {
      error = ERROR_NC;
    }
  }

  if (error == OK) {
    int size = A->rows;

    if (size == 1) {
      *result = A->matrix[0][0];
    } else if (size == 2) {
      *result =
          A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    } else {
      *result = 0.0;
      for (int j = 0; j < size && error == OK; j++) {
        matrix_t minor;
        error = create_matrix(size - 1, size - 1, &minor);

        if (error == OK) {
          int mi = 0;
          for (int i = 1; i < size && error == OK; i++) {
            int mj = 0;
            for (int k = 0; k < size && error == OK; k++) {
              if (k != j) {
                minor.matrix[mi][mj] = A->matrix[i][k];
                mj++;
              }
            }
            mi++;
          }

          if (error == OK) {
            double det_minor = 0.0;
            error = determinant(&minor, &det_minor);
            if (error == OK) {
              double sign = (j % 2 == 0) ? 1.0 : -1.0;
              *result += sign * A->matrix[0][j] * det_minor;
            }
          }
          remove_matrix(&minor);
        }
      }
    }
  }

  return error;
}

int inverse_matrix(matrix_t *A, matrix_t *result) {
  int error = OK;
  double det = 0.0;
  if (error == OK) {
    if (!arguments_check(A) || result == NULL || A->rows != A->columns) {
      error = ERROR_NC;
    }
  }
  if (error == OK) {
    error = determinant(A, &det);
    if (error == OK && fabs(det) < EPSILON) {
      error = ERROR_CL;
    }
  }
  if (error == OK && A->rows == 1) {
    error = create_matrix(1, 1, result);
    if (error == OK) {
      result->matrix[0][0] = 1.0 / A->matrix[0][0];
    }
  }
  if (error == OK && A->rows > 1) {
    matrix_t complements;
    matrix_t transposed;

    complements.matrix = NULL;
    complements.rows = 0;
    complements.columns = 0;
    transposed.matrix = NULL;
    transposed.rows = 0;
    transposed.columns = 0;

    if (error == OK) {
      error = calc_complements(A, &complements);
    }
    if (error == OK) {
      error = transpose(&complements, &transposed);
    }
    if (error == OK) {
      error = create_matrix(A->rows, A->columns, result);
    }
    if (error == OK) {
      for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->columns; j++) {
          result->matrix[i][j] = transposed.matrix[i][j] / det;
        }
      }
    }
    if (complements.matrix != NULL) {
      remove_matrix(&complements);
    }
    if (transposed.matrix != NULL) {
      remove_matrix(&transposed);
    }
  }

  return error;
}