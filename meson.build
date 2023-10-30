project('Subset Sum Problem', 'cpp',
  version : '0.9')

# sqlite_dep = dependency('sqlite3')
# draw_dep = dependency('libxml-2.0', required: true)

# Main executable
# solve = ['src/main/execs/SSP.cpp']
# executable('solve', solve, c_args : ['-g'])

# Read executable
read = ['src/main/ssp/execs/Read.cpp']
executable('read', read, c_args : ['-g'])

# Eval a sol
eval = ['src/main/ssp/execs/Eval.cpp']
executable('eval', eval, c_args : ['-g'])

# Prints full list of massses
populate = ['src/main/ssp/execs/Populate.cpp']
executable('populate', populate, c_args : ['-g'])

# Coords
coords = ['src/main/ssp/execs/Coords.cpp']
executable('coords', coords, c_args : ['-g'])


# #FALTA SWEEP
# drawfile = ['src/main//execs/Draw.cpp']
# executable('draw', drawfile, dependencies : [sqlite_dep, draw_dep], c_args : ['-g'])

# #Draw
# drawfile = ['src/main//execs/Draw.cpp']
# executable('draw', drawfile, dependencies : [sqlite_dep, draw_dep], c_args : ['-g'])

# #Script
# runfile = ['src/main/execs/Run.cpp']
# executable('script', runfile, dependencies : [sqlite_dep], c_args : ['-g'])

# #Run
# runfile = ['src/main//execs/Create_sols.cpp']
# executable('run', runfile, dependencies : [sqlite_dep], c_args : ['-g'])

# #Solve
# solvefile = ['src/main//execs/Solve.cpp']
# executable('solve', solvefile, dependencies : [sqlite_dep], c_args : ['-g'])
