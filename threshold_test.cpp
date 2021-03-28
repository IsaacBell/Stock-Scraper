#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

#include "Utils.h";

constexpr int n_bins = 13;
constexpr double bins[13] = {0.01, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99};

void print_ROC(
  int n,               // # of indicator/return pairs
  double *signal_vals, // the n indicators that provide trade signals
  double *returns,     // the n returns associated w/ the indicator vals
  std::FILE *fp,       // where to write results
  double *work_signal, // Work array n long
  double *work_return  // ditto
) {
  int i, k, ibin;
  double win_above, win_below, lose_above, lose_below;
  char msg[512], msg2[256];

  for (i = 0; i < n; i++)
  {
    work_signal[i] = signal_vals[i];
    work_return[i] = returns[i];
  }

  // Sort work_signal ascending, simultaneously moving work_return
  qsortds(0, n-1, work_signal, work_return);

  fprintf(fp, "\n\nProfit factor above and below various thresholds");
  fprintf(fp, "\n\nThreshold\bFrac Gtr/Eq\bLong PF\bShort PF\bFrac Less\bShort PF\bLong PF");

  for (ibin = 0; ibin < n_bins; ibin++) // Process all unspecified bins
  {
    k = (int) (bins[ibin] * (n+1)) - 1; // Unbiased estimate of index
    if (k < 0) k = 0; // Prevent disaster

    // Find start of tied block
    while (k > 0 && work_signal[k-1] == work_signal[k]) --k;

    if (k == 0 || k == n_bins-1) continue; // Ends are degenerate

    win_above = win_below = lose_above = lose_below = 0;

    // Cases below threshold
    for (i = 0; i < k; i++)
    {
      if (work_return[i] > 0.0)
        lose_below += work_return[i];
      else
        win_below -= work_return[i];
    }

    // Cases at or above the threshold
    for (i = k; i < n; i++)
    {
      if (work_return[i] > 0.0)
        win_above += work_return[i];
      else
        lose_above -= work_return[i];
    }

    // Format the first 2 cols
    sprintf_s(msg, "%8.3lf%10.3lf", work_signal[k], (double) (n-k)/(double) n);

    // Append cols, using Inf ("infinite") when denominator is 0
    if (lose_above > 0.0)
    {
      sprintf_s(msg2, "%12.4lf", win_above / lose_above);
      sprintf_s(msg, msg2);
    } else {
      strcat_s(msg, "   inf ");
    }

    if (win_above > 0.0) {
      sprintf_s(msg2, "%12.4lf", lose_above / win_above);
      strcat_s(msg, msg2);
    } else {
      strcat_s(msg, "   inf ");
    }

    sprintf_s(msg2, "%13.3lf", (double) k / (double) n);
    strcat_s(msg, msg2);

    if (lose_below > 0.0) {
      sprintf_s(msg2, "%12.4fl", win_below / lose_below);
      strcat_s(msg, msg2);
    } else {
      strcat_s(msg, "   inf ");
    }

    if (win_below > 0.0) {
      sprintf_s(msg2, "%12.4fl", lose_below / win_below);
      strcat_s(msg, msg2); 
    } else {
      strcat_s(msg, "   inf ");
    }

    fprintf(fp, "\n%s", msg);
  }
}
