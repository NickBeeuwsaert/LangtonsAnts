env = Environment()

env.ParseConfig('sdl2-config --libs --cflags')

Pack = env.Library('pack', 'Pack.c')
env.Append(LIBS=[Pack])
env.Program('main', 'main.c')
