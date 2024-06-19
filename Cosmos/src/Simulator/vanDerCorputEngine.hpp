/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2012 LSV & LACL                                          *
 * Authors: Benoît Barbot & Paolo Ballarini & Hilal Djafri                     *
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
 * file vanDerCorputEngine.hpp created by benoit barbot on 01/02/2018          *
 *******************************************************************************
 */

#ifndef vanDerCorputEngine_hpp
#define vanDerCorputEngine_hpp

#include <boost/random.hpp>
#include <stdio.h>
/*
template<typename IntType, IntType a, IntType c, IntType m>
class van_der_corput_engine {
public:
    // types
    typedef IntType result_type;

    // construct/copy/destruct
    linear_congruential_engine();
    explicit linear_congruential_engine(IntType);
    template<typename SeedSeq> explicit linear_congruential_engine(SeedSeq &);
    template<typename It> linear_congruential_engine(It &, It);

    // public member functions
    void seed();
    void seed(IntType);
    template<typename SeedSeq> void seed(SeedSeq &);
    template<typename It> void seed(It &, It);
    IntType operator()();
    template<typename Iter> void generate(Iter, Iter);
    void discard(boost::uintmax_t);

    // public static functions
    static result_type min();
    static result_type max();

    // friend functions
    template<typename CharT, typename Traits>
    friend std::basic_ostream< CharT, Traits > &
    operator<<(std::basic_ostream< CharT, Traits > &,
               const linear_congruential_engine &);
    template<typename CharT, typename Traits>
    friend std::basic_istream< CharT, Traits > &
    operator>>(std::basic_istream< CharT, Traits > &,
               linear_congruential_engine &);

    // public data members
    static const bool has_fixed_range;
    static const IntType multiplier;
    static const IntType increment;
    static const IntType modulus;
    static const IntType default_seed;
};*/

#endif /* vanDerCorputEngine_hpp */
