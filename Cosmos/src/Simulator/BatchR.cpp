/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2012 LSV & LACL                                          *
 * Authors: Paolo Ballarini Benoît Barbot & Hilal Djafri                       *
 * Website: http://www.lsv.ens-cachan.fr/Software/cosmos                       *
 *                                                                             *
 * This program is free software; you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by        *
 * the Free Software Foundation; either version 3 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * This program is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU General Public License for more details.                                *
 *                                                                             *
 * You should have received a copy of the GNU General Public License along     *
 * with this program; if not, write to the Free Software Foundation, Inc.,     *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
 * file BatchR.cpp created by Benoit Barbot.                                   *
 *******************************************************************************
 */

#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include "BatchR.hpp"

using namespace std;

/**
 *Initialize the batch with zeros
 * @param i is the number of formula evaluated by the automaton.
 */
BatchR::BatchR(size_t i, size_t j)
    : I(0), Isucc(0), IsBernoulli(vector<bool>(i, true)),
      Mean(vector<double>(i, 0.0)), M2(vector<double>(i, 0.0)),
      M3(vector<double>(i, 0.0)), M4(vector<double>(i, 0.0)),
      Min(vector<double>(i, DBL_MAX)), Max(vector<double>(i, -DBL_MAX)),
      bernVar(j, false), simTime(0.0) {}

/**
 * Add a simulation to the batch
 * If the simulation is a success then The Mean and second Moment are updated
 * @param Result the result of one trajectory in the simulator.
 */
void BatchR::addSim(const SimOutput &Result) {
  I++;
  if (Result.accept) {
    Isucc++;

    for (size_t i = 0; i < bernVar.size(); i++) {
      if (Result.qualR[i])
        bernVar[i]++;
    }

    for (size_t i = 0; i < Mean.size(); i++) {

      if (Result.quantR[i] * (1 - Result.quantR[i]) != 0) {
        IsBernoulli[i] = false;
      }

      double temp = Result.quantR[i];
      Mean[i] += temp;
      temp *= Result.quantR[i];
      M2[i] += temp;
      temp *= Result.quantR[i];
      M3[i] += temp;
      temp *= Result.quantR[i];
      M4[i] += temp;
      Min[i] = min(Min[i], Result.quantR[i]);
      Max[i] = max(Max[i], Result.quantR[i]);
    }
  }
}

/**
 * Take the union of two batch of result.
 * @param batch is a batch of result wich is added to the current batch.
 */
void BatchR::unionR(const BatchR &batch) {

  I += batch.I;
  Isucc += batch.Isucc;

  if (batch.Mean.size() > Mean.size()) {
    size_t TableLength = batch.Mean.size();
    IsBernoulli.resize(TableLength, true);
    Mean.resize(TableLength, 0.0);
    M2.resize(TableLength, 0.0);
    M3.resize(TableLength, 0.0);
    M4.resize(TableLength, 0.0);
    Min.resize(TableLength, DBL_MAX);
    Max.resize(TableLength, -DBL_MAX);
  }
  for (size_t i = 0; i < Mean.size(); i++) {
    IsBernoulli[i] = IsBernoulli[i] && batch.IsBernoulli[i];

    Mean[i] += batch.Mean[i];
    M2[i] += batch.M2[i];
    M3[i] += batch.M3[i];
    M4[i] += batch.M4[i];
    Min[i] = fmin(Min[i], batch.Min[i]);
    Max[i] = fmax(Max[i], batch.Max[i]);
  }
  if (batch.bernVar.size() > bernVar.size())
    bernVar.resize(batch.bernVar.size(), 0);

  for (size_t i = 0; i < bernVar.size(); i++)
    bernVar[i] += batch.bernVar[i];

  simTime += batch.simTime;
}

/**
 * This function write a batch on the standart output it is suposed to
 * be read by the function BatchR::inputR
 */
void BatchR::outputR(ostream &f) {
  // boost::archive::text_oarchive foa(f);

  // foa << *this;

  unsigned long int check = 8427; // magic value
  f.write(reinterpret_cast<char *>(&check), sizeof(check));
  f.write(reinterpret_cast<char *>(&I), sizeof(I));
  f.write(reinterpret_cast<char *>(&Isucc), sizeof(Isucc));
  size_t v = Mean.size();
  f.write(reinterpret_cast<char *>(&v), sizeof(size_t));
  v = bernVar.size();
  f.write(reinterpret_cast<char *>(&v), sizeof(size_t));

  for (unsigned int i = 0; i < Mean.size(); i++) {
    bool tmpbool = IsBernoulli[i];
    f.write(reinterpret_cast<char *>(&tmpbool), sizeof(bool));
    f.write(reinterpret_cast<char *>(&Mean[i]), sizeof(Mean[0]));
    f.write(reinterpret_cast<char *>(&M2[i]), sizeof(Mean[0]));
    f.write(reinterpret_cast<char *>(&M3[i]), sizeof(Mean[0]));
    f.write(reinterpret_cast<char *>(&M4[i]), sizeof(Mean[0]));
    f.write(reinterpret_cast<char *>(&Min[i]), sizeof(Mean[0]));
    f.write(reinterpret_cast<char *>(&Max[i]), sizeof(Mean[0]));
  }
  for (unsigned int i = 0; i < bernVar.size(); i++)
    f.write(reinterpret_cast<char *>(&bernVar[i]), sizeof(bernVar[0]));
  f.write(reinterpret_cast<char *>(&simTime), sizeof(simTime));

  f.flush();
}

/**
 * Read a batch from a file.
 * The batch should be print with BatchR::outputR
 * @param f an opened for reading file.
 * @return true if the read was succesfull and false if it was not complete.
 */
bool BatchR::inputR(FILE *f) {
  bool readb;
  size_t readbyte;
  bool ok = true;

  unsigned long int check = 0;
  readbyte = fread(reinterpret_cast<char *>(&check), sizeof check, 1, f);
  ok &= (check == 8427); // check magic value
  ok &= (readbyte == 1);

  readbyte = fread(reinterpret_cast<char *>(&I), sizeof I, 1, f);
  ok &= (readbyte == 1);

  readbyte = fread(reinterpret_cast<char *>(&Isucc), sizeof Isucc, 1, f);
  ok &= (readbyte == 1);

  size_t TableLength;
  readbyte =
      fread(reinterpret_cast<char *>(&TableLength), sizeof TableLength, 1, f);
  ok &= (readbyte == 1);

  size_t nbBernouilli;
  readbyte =
      fread(reinterpret_cast<char *>(&nbBernouilli), sizeof nbBernouilli, 1, f);
  ok &= (readbyte == 1);
  if (!ok)
    return false;

  if (Mean.size() != TableLength) {
    // cerr << "TableLength:" << TableLength << "resize from " << Mean.size() <<
    // endl;
    IsBernoulli.resize(TableLength);
    Mean.resize(TableLength);
    M2.resize(TableLength);
    M3.resize(TableLength);
    M4.resize(TableLength);
    Min.resize(TableLength);
    Max.resize(TableLength);
  }

  for (unsigned int i = 0; i < TableLength; i++) {
    readbyte = fread(reinterpret_cast<char *>(&readb), sizeof readb, 1, f);
    IsBernoulli[i] = readb;
    ok &= (readbyte == 1);
    readbyte =
        fread(reinterpret_cast<char *>(&(Mean[i])), sizeof Mean[i], 1, f);
    ok &= (readbyte == 1);
    readbyte = fread(reinterpret_cast<char *>(&(M2[i])), sizeof M2[i], 1, f);
    ok &= (readbyte == 1);
    readbyte = fread(reinterpret_cast<char *>(&(M3[i])), sizeof M2[i], 1, f);
    ok &= (readbyte == 1);
    readbyte = fread(reinterpret_cast<char *>(&(M4[i])), sizeof M2[i], 1, f);
    ok &= (readbyte == 1);
    readbyte = fread(reinterpret_cast<char *>(&(Min[i])), sizeof Min[i], 1, f);
    ok &= (readbyte == 1);
    readbyte = fread(reinterpret_cast<char *>(&(Max[i])), sizeof Max[i], 1, f);
    ok &= (readbyte == 1);
  }

  if (bernVar.size() != nbBernouilli)
    bernVar.resize(nbBernouilli);
  for (size_t i = 0; i < nbBernouilli; i++) {
    readbyte =
        fread(reinterpret_cast<char *>(&bernVar[i]), sizeof bernVar[0], 1, f);
    ok &= (readbyte == 1);
  }

  readbyte = fread(reinterpret_cast<char *>(&simTime), sizeof simTime, 1, f);
  ok &= (readbyte == 1);

  return ok;
}

/*
template<class Archive>
void BatchR::serialize(Archive & ar, const unsigned int)
{
    ar & I & Isucc & IsBernoulli & Mean & M2 & M3 & M4 & Min & Max & bernVar &
simTime;
}*/

ostream &operator<<(ostream &os, const BatchR &b) {
  os << "I:\t" << b.I << endl
     << "Isucc:\t" << b.Isucc << endl
     << "TableLength:\t" << b.Mean.size() << endl;
  for (size_t i = 0; i < b.Mean.size(); i++) {
    os << "Mean:\t" << b.Mean[i] << endl
       << "M2:\t" << b.M2[i] << endl
       << "M3:\t" << b.M3[i] << endl
       << "M4:\t" << b.M4[i] << "Min:\t" << b.Min[i] << endl
       << "Max:\t" << b.Max[i] << endl
       << "IsBernoulli:\t" << b.IsBernoulli[i] << endl;
  }
  for (size_t i = 0; i < b.bernVar.size(); i++) {
    os << "Bernouilli:\t" << b.bernVar[i] << endl;
  }
  os << "Simulation Time:\t" << b.simTime << endl;
  return os;
}
