spfi
=====

Fork "spfi" to create a new MOOSE-based application.

For more information see: [https://mooseframework.org/getting_started/new_users.html#create-an-app](https://mooseframework.org/getting_started/new_users.html#create-an-app)

git clone https://github.com/Sachinscnpdl/spfi.git
cd spfi

activate moose environment
make j -8



# (1) ensure mamba available in base
conda install -n base -c conda-forge mamba -y

# (2) create the env with explicit packages/builds and a stable Python
mamba create -n moose \
  -c https://conda.software.inl.gov/public -c conda-forge \
  "python=3.10" \
  "mpich" \
  "moose-dev=2025.04.22=mpich" \
  "moose-libmesh=2025.04.22=mpich_0" \
  "moose-peacock=2025.04.17.*" \
  "moose-tools=2025.04.17.*" \
  "moose-wasp=2025.02.25.*" -y

# then activate
conda activate moose
