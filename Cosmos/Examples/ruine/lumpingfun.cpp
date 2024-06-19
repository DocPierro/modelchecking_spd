

void SPN::lumpingFun(vector<int> *vect) {
  int l2 = l;
  vector<int> vect2(l2, 0);
  for (int i = 0; i < n * l; i++) {
    vect2[i % l2] += (*vect)[i];
  }

  vect->resize(l2);
  for (int i = 0; i < l2; i++) {
    (*vect)[i] = vect2[i];
  }
}

double REHandling::direct_computation(const abstractMarking &Marking) {
  return -1.0;
}
