## The SUrrogate MOdeling (SUMO) Toolbox - README

### Introduction

The Surrogate Modeling Toolbox (SUMO Toolbox) is a Matlab toolbox that automatically builds accurate surrogate models (also known as metamodels or response surface models) of a given data source (simulation code, data set, script, ...) within the accuracy and time constraints set by the user. In doing so the toolbox minimizes the number of data points (which it chooses automatically) since they are usually expensive. The toolbox tries to be as adaptive and autonomous as possible, requiring no user input besides some initial configuration.

![Metamodel generation](http://sumowiki.intec.ugent.be/images/thumb/8/82/Metamodel-generation.png/500px-Metamodel-generation.png "SUMO-Toolbox : Generating an approximation for a reference model")

However, since there is no such thing as a one-size-fits-all, the toolbox has been designed to be fully pluggable and extensible using standard object oriented design patterns. Implementations of the different components (model types, sampling strategies, model selection criteria, hyperparameter optimization algorithms,...) can be plugged-in, compared, or replaced by custom implementations. In this way the SUMO Toolbox provides a common platform to easily test and benchmark different sampling and approximation strategies while easily integrating in the engineering design process.

### Download

See [download page](http://sumowiki.intec.ugent.be/index.php/Downloading)

### Documentation

To get started with the SUMO toolbox please see the [getting started page](http://sumowiki.intec.ugent.be/index.php/Running#Getting_started).
Full documentation can be found at the [wiki](http://sumowiki.intec.ugent.be)

### Contribute

Suggestions on how to improve the SUMO toolbox are always welcome. For more information please see
the [feedback page](http://sumowiki.intec.ugent.be/index.php/Feedback)

### Changelog

[CHANGELOG](CHANGELOG.txt)

### Copyright

[LICENSE](LICENSE.txt)

[NOTICE](NOTICE.txt)
