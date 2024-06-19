#!/bin/bash

mkdir -p ../bin
rm -rf tmp
#ocamlbuild -use-ocamlfind -j 4 -pp "camlp4o pa_macro.cmo -DHAS_XML -DHAS_ZIP" -I ClessParser prism2SPT.native modelConvert.native parserUnparseAttribute.native
ocamlbuild -use-ocamlfind -j 4 -I ClessParser -I Prism -I Semantics -tag 'debug' prism2SPT.native StochasticPetriNet.cmo modelConvert.native stochasticBound.native stochasticBound.byte parserUnparseAttribute.native parserUnparseAttribute.byte modelConvert.byte CosmosSimulink.native CosmosSimulink.byte
#cd _build
#ocamlmktop type.cmo prismType.cmo parserPrism.cmo lexerPrism.cmo PetriNet.cmo StochPTPrinter.cmo SemanticPT.cmo StochasticPetriNet.cmo -o petriscript
#cd ..

cp modelConvert.native ../bin/modelConvert
cp CosmosSimulink.native ../bin/CosmosSimulink
cp stochasticBound.native ../bin/stochasticBound
