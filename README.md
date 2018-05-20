# ts-analyzer
Visualize content of a transport stream.

## How to run it
Install Qt Creator or Qt SDK and open the ts-analyzer.pro project file.
From the Qt Creator IDE you can now edit this project.

You also need to load the ts-lib submodule
```
git submodule init
git submodule update
```
or
```
git submodule update --init
```
in order to be able to run this project successfully.

## Git stategy
Note we use the most advanced git strategy NVIE. Please consult
CM before you commit to this repo. In short there are 2 type categories
of branche *main branches* and *supporting branches*.


### Main branches
The central repo holds two main branches with an infinite lifetime:
* master
* develop

The master branch at origin should be familiar to every Git user.
Parallel to the master branch, another branch exists called develop.

We consider origin/master to be the main branch where the source code of
HEAD always reflects a production-ready state.

We consider origin/develop to be the main branch where the source code of
HEAD always reflects a state with the latest delivered development changes
for the next release. Some would call this the “integration branch”.
This is where any automatic nightly builds are built from.

### Supporting branches
Next to the main branches master and develop, our development model uses
a variety of supporting branches to aid parallel development between
team members, ease tracking of features, prepare for production releases
and to assist in quickly fixing live production problems.
Unlike the main branches, these branches always have a limited life time,
since they will be removed eventually.

The different types of branches we may use are:

* Feature branches
* Release branches
* Hotfix branches

This image shows the branching strategy:
![Git branching model](https://github.com/skullanbones/ts-analyzer/tree/master/images/git-model@2x.png)
