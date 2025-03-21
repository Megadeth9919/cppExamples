#include <memory>
#define OFFSET(x, y, stride) (x * stride + y)
void matmul(float* A, float *B, float *C, int M, int K, int N) {

  int A_start = 0, B_start = 0, C_start = 0;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      B_start = 0;
      // C[OFFSET(i, j, N)] = 0;
      C[C_start + j] = 0;
      for (int k = 0; k < K; k++) {
        C[C_start + j] += A[A_start + k] * B[B_start + j];
        B_start += N;
      }
    }
    A_start += K;
    C_start += N;
  }
}

#define OFFSET(x, y, stride) (x * stride + y)
#define BLOCK_SIZE 32 // 假设块大小为32x32

void matmul_opt(float* A, float *B, float *C, int M, int K, int N) {
  float blockA[BLOCK_SIZE][BLOCK_SIZE];
  float blockB[BLOCK_SIZE][BLOCK_SIZE];
  float blockC[BLOCK_SIZE][BLOCK_SIZE] = {0}; // 初始化块为0

  for (int i = 0; i < M; i += BLOCK_SIZE) {
    for (int j = 0; j < N; j += BLOCK_SIZE) {
      for (int ii = 0; ii < BLOCK_SIZE && ii < M - i; ++ii) {
        for (int jj = 0; jj < BLOCK_SIZE && jj < N - j; ++jj) {
          blockC[ii][jj] = 0; // 初始化当前块的每个元素为0
          for (int k = 0; k < K; k += BLOCK_SIZE) {
            for (int iii = 0; iii < BLOCK_SIZE && iii < K - k; ++iii) {
              for (int jjj = 0; jjj < BLOCK_SIZE && jjj < K - k; ++jjj) {
                blockA[ii][iii] = A[OFFSET(i + ii, k + iii, K)];
                blockB[jjj][jj] = B[OFFSET(k + iii, j + jj, N)];
              }
              for (int iii = 0; iii < BLOCK_SIZE && iii < K - k; ++iii) {
                for (int jjj = 0; jjj < BLOCK_SIZE && jjj < N - j; ++jjj) {
                  blockC[ii][jj] += blockA[ii][iii] * blockB[iii][jj + jjj];
                }
              }
            }
          }
        }
      }
      for (int ii = 0; ii < BLOCK_SIZE && ii < M - i; ++ii) {
        for (int jj = 0; jj < BLOCK_SIZE && jj < N - j; ++jj) {
          C[OFFSET(i + ii, j + jj, N)] = blockC[ii][jj];
        }
      }
    }
  }
}

int main() {
  return 0;
}