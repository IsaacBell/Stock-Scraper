#include <ctime>
#include <stdlib.h>

/*
  One of the most important applications of the MCPT is in screening
  large #s of candidate indicators to identify a much smaller, promising
  subset. Then there is a substantial probability that 1+ competitors will
  be lucky enough to rise to the top despite having little to no real value.

  MCPT lets us detect this and avoid being fooled. This code references an
  optimal-threshold example. If we have not made up our mind in advance that we're
  interested only in a long or short system, we'll considering two performance
  and p-value results, one for short systems w/ an optimal low threshold, one
  for long systems w/ an optimal high threshold. 
  
  We'll favor whichever has better performance. Then we note the better of 
  the 2 systems on the original, unpermuted optimization, and do the same 
  selection for each permuted run, comparing the better of each permuted 
  run to the better of the original run.
*/

// Reset the random number generator with the system clock.
void seed()
{
  srand(time(0));
}

// Generate a random number between 0 and 1
// return a uniform number in [0,1].
template <typename T=double>
T unifRand()
{
  return static_cast<T>(rand()) / T(RAND_MAX);
}

// Generate a random number in a real interval.
// param a one end point of the interval
// param b the other end of the interval
// return a inform rand numberin [a,b].
template <typename T=double>
T unifRand(T a, T b)
{
  return (b-a)*unifRand<T>() + a;
}

///////////// UNUSED ///////////////
// Generate a random integer between 1 and a given value.
// param n the largest value 
// return a uniform random value in [1,...,n]
long unifRand(long n)
{
    
  if (n < 0) n = -n;
  if (n==0) return 0;
  /* There is a slight error in that this code can produce a return value of n+1
  **
  **  return long(unifRand()*n) + 1;
  */
  //Fixed code
  long guard = (long) (unifRand() * n) +1;
  return (guard > n)? n : guard;
}

void opt_thresh(
  int n,
  int min_kept,
  int flip_sign,
  double *signal_vals,
  double *work_permute,
  double *pf_all, // profit factor (abbrv. pf) => sum # wins / sum # losses
  double &long_thresh,
  double &pf_long,
  double &short_thresh,
  double &pf_short,
  double *work_signal,
  double *work_return
) {
  // This code assumes you'll produce 2 trading systems, 1 long & 1 short
  // If not, just delete the long/short code and put 1 sys
  return;
};

// Most params have nothing directly to do with the MCPT
// We just need one extra work area for the MCPT, to store
// returns and permute them. It would be rude to permute
// returns that were supplied by the caller
void opt_MCPT(
  int n,                // indicator/return pairs
  int min_kept,         // must keep (trade) at least this many cases
  int flip_sign,        // if non-zero, flip indicator sign
  int n_reps,           // # replications, including non-permuted
  double *signal_vals,  // indicators
  double *returns,      // assoc. returns,
  double *pf_all,       // profit factor (abbrv. pf) => sum # wins / sum # losses
  double *high_thresh,
  double *pf_high,      // profit factor >= threshold
  double *low_thresh,   // for long trades
  double *pf_low,       // pf < thresh
  double *pval_long,    // p-value for long trades
  double *pval_short,   // p-value for short trades
  double *pval_best,    // p-value for best-side trades
  double *work_signal,  // work area n long
  double *work_return,  // ""
  double *work_permute  // ""
) {
  seed();
  double original_best_pf = 0,
         long_count       = 0,
         short_count      = 0,
         best_count       = 0;


  // Copy returns to our safe work area
  // Outer loop processes replications, including thee 1st, which isn't permuted
  
  // The random permutation block is the std algo - fast & correct
  // Shuffling is skipped on the 1st replication (irep=0) but done thereafter

  // Many uniform random generators guarantee that value 1.0 will nevr be returned
  // This code checks for that event and prevents failure

  for (int i = 0; i < n; i++)
    work_permute[i] = returns[i];

  for (int irep = 0; irep < n_reps; irep++)
  {
    if (irep) { // Shuffle replication?
      int i = n;
      while (i > 1) { // While at least 2 to shuffle
        int j = (int) (unifRand() * i);
        if (j>= 1) [[unlikely]] // Avoid disaster if uniform is 1.0
          j = i-1;

        int dtemp = work_permute[--i];
        work_permute[i] = work_permute[j];
        work_permute[j] = dtemp;
      }
    }

    // This subroutine optimizes a pair of trading systems (1 long, 1 short)
    // and returns performance figures for each. Here we are optimizing trade
    // threshold per single indicator, but that's not required.

    // Subroutine can optimize as many params as desired, and as few as 1 sys
    // Optimal params are ignored for the MCPT, returned to caller. 

    // All we need here the 2 performance figures, pf_long and pf_short.
    // Since we're doing a best-of test we also find the better of 2 performances

    opt_thresh(
      n,
      min_kept,
      flip_sign,
      signal_vals,
      work_permute,
      pf_all,
      &long_thresh,
      &pf_long,
      &short_thresh,
      &pf_short,
      work_signal,
      work_return
    );

    
    double best_pf = (pf_long > pf_short) ? pf_long : pf_short;

    // if this is the 1st, unpermuted replication
    if (irep == 0) {
      *high_thresh = long_thresh;
      *pf_high     = pf_long
      *low_thresh  = short_thresh;
      *pf_low      = pf_short;
      original_best_pf = best_pf;
      int long_cnt = short_count = best_count = 1;
    } else {
      // if this is a permuted replic, for each p-value
      // we count the # times the permuted performance >= original

      if (pf_long >= *pf_high) ++long_count;
      if (pf_short >= *pf_low) ++short_count;
      if (best_pf >= original_best_pf) ++best_count;
    }

    // Divide each counter by the total # replications (including the
    // unpermuted rep) to get the associated p-values

    *pval_long  = (double) long_count  / (double) n_reps;
    *pval_short = (double) short_count / (double) n_reps;
    *pval_best  = (double) best_count  / (double) n_reps;
  }
}
