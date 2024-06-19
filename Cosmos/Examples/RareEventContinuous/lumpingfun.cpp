

void SPN::lumpingFun(vector<int> *vect) {
  int reste = 0;
  for (int i = vect->size() - 1; i > 0; i--) {
    if ((*vect)[i] > r) {
      reste += (*vect)[i] - r;
      (*vect)[i] = r;
    } else if ((*vect)[i] + reste > r) {
      reste -= (r - (*vect)[i]);
      (*vect)[i] = r;
    } else {
      (*vect)[i] += reste;
      reste = 0;
    }
  }
  (*vect)[0] += reste;
}
