# Weighted sampling without replacement

An implementation of the CK Wong & M. Easton algorithm for weighted sampling without replacement [paper](https://www.researchgate.net/publication/220617264_An_Efficient_Method_for_Weighted_Sampling_Without_Replacement?enrichId=rgreq-e42f11ef98e6b1a82cacfdc5209e459d-XXX&enrichSource=Y292ZXJQYWdlOzIyMDYxNzI2NDtBUzoxMTAyMzgyODgwNjA0MTZAMTQwMzI5NDI2NDM5MQ%3D%3D&el=1_x_2&_esc=publicationCoverPdf) in pure C.

The Garsia-Wachs algorithm is used for constructing the underlying balanced tree, for details see sec. 6.2.2 in "The Art of Computer Programming. Vol. 3: Searching and Sorting" by Donald Knuth.

# Usage

The general workflow is to create a sampler using the `get_sampler(double* probs, int N)` function, where `probs` is a pointer to an array of doubles of length `N`. The values in the `probs` array should represent probabilities (not weights), meaning they should be in the range (0,1) and sum to 1.0.

After creating a sampler, you can pass it repeatedly to function `sample_wor(SamplerState* state)` which returns a sampled index into the `probs` array each time, so an integer value between 0 and N-1.

Note, that by default the sampler will crash the program if you try to sample more than N values. To make it return a NULL instead comment out the `-DABORT_ON_OVERSAMPLE` flag in the Makefile before building.

For an example of using the sampler see `test.c`.
