#
import os


def files(rootpath, ext):
    out = []
    for _root, _dirs, _files in os.walk(rootpath):
        for f in _files:
            if f.endswith(ext):
                fpth = os.path.relpath(os.path.join(_root, f))
                out.append(fpth.replace('\\', '/'))
    return out


def dirs(rootpath, ext):
    out = []
    fls = files(rootpath, ext)
    for f in fls:
        dr = os.path.dirname(f)
        if dr not in out:
            out.append(dr.replace('\\', '/'))
    return out


if __name__ == '__main__':
    rootpath = os.path.abspath(os.path.dirname(__file__))
    print('rootpath: ', rootpath)

    print('\nSOURCES DIRS: ', '*' * 100, '\n')
    [print('%s \\' % el) for el in dirs(rootpath, '.c')]
    print('\nSOURCES FILES: ', '*' * 100, '\n')
    [print('%s \\' % el) for el in files(rootpath, '.c')]

    print('\nINCLUDES DIRS: ', '*' * 100)
    [print('-I%s \\' % el) for el in dirs(rootpath, '.h')]
