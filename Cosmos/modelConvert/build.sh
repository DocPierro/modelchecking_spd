#!/bin/bash

mkdir -p ../bin
rm -rf tmp
#ocamlbuild -use-ocamlfind -j 4 -pp "camlp4o pa_macro.cmo -DHAS_XML -DHAS_ZIP" -I ClessParser prism2SPT.native modelConvert.native parserUnparseAttribute.native
ocamlbuild -use-ocamlfind -j 4 -I ClessParser -I Prism -I Semantics prism2SPT.native StochasticPetriNet.cmo SemanticPT.cmo  StochasticSymmetricNet.cmo modelConvert.native stochasticBound.native parserUnparseAttribute.native modelConvert.byte CosmosSimulink.native prismTrans.cma prismTrans.cmxa

ocamlmktop -o petriscript -I _build str.cma type.cmo Prism/prismType.cmo Prism/parserPrism.cmo Prism/lexerPrism.cmo PetriNet.cmo StochPTPrinter.cmo Semantics/SemanticPT.cmo StochasticPetriNet.cmo StochasticSymmetricNet.cmo


cp modelConvert.native ../bin/modelConvert
cp CosmosSimulink.native ../bin/CosmosSimulink
cp stochasticBound.native ../bin/stochasticBound
