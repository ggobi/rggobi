## Test environments
* local OS X install, R 3.3.1
* ubuntu 12.04 (on travis-ci), R 3.3.1
* win-builder (devel and release)

## R CMD check results

0 errors | 0 warnings | 1 note

* checking foreign function calls ... NOTE
  Registration problem:
  Evaluating ‘.ggobi.symbol(.name)’ during check gives error
  
  This is a false positive - the package manually adds a prefix to
  every C function name in order (I presume) to avoid conflicts. This
  is no longer necessary but it would be a lot of work to fix.

## Reverse dependencies

I did not re-check the revdeps because this version just fixes R CMD check issues.
