--- PART 1: Correctness Verification (3x4 sample image) ---
Input integer matrix:
  64   89  114   84
 140  166  191   84
 216  242   38   84

Assembly converted single-precision float matrix:
   0.25    0.35    0.45    0.33
   0.55    0.65    0.75    0.33
   0.85    0.95    0.15    0.33

Result Check vs C execution: SUCCESS

--- PART 2: Performance Profiling (30-run average per size) ---
Total Pixels |    Dimensions | Correctness Check |       Avg Run Time
------------------------------------------------------------------------
       100 |     10x10     |          PASSED |           0.000323 ms
     10000 |    100x100    |          PASSED |           0.029440 ms
   1000000 |   1000x1000   |          PASSED |           2.866137 ms
------------------------------------------------------------------------
