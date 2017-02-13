import glob
import os
import shutil
import sys
import zipfile


project_dir = ''
asset_dir = ''
lib_dir = ''
scripts_dir = ''
build_type = ''
build_location = ''


def build_assets():
    current_dir = os.getcwd()

    os.chdir(scripts_dir)

    if not os.path.isdir('./tmp'):
        os.mkdir('./tmp')

    asset_package = zipfile.ZipFile('./tmp/assets', mode='w', compression=zipfile.ZIP_DEFLATED)
    os.chdir(asset_dir)
    asset_files = glob.glob('**', recursive=True)
    for filename in asset_files:
        asset_package.write(filename)
    asset_package.close()

    os.chdir(current_dir)


def copy_assets():
    shutil.copy(os.path.join(scripts_dir, 'tmp/assets'), build_location)


def copy_runtime():
    if build_type == '32':
        shutil.copy(os.path.join(lib_dir, 'SDL2-2.0.5/lib/x86/SDL2.dll'), build_location)
    elif build_type == '64':
        shutil.copy(os.path.join(lib_dir, 'SDL2-2.0.5/lib/x64/SDL2.dll'), build_location)
    else:
        sys.exit('invalid build type')


if __name__ == '__main__':
    if len(sys.argv) != 4:
        sys.exit('invalid arguments')

    project_dir = os.path.abspath(sys.argv[1])
    build_type = sys.argv[2]
    build_location = os.path.abspath(sys.argv[3])

    asset_dir = os.path.join(project_dir, 'assets')
    lib_dir = os.path.join(project_dir, 'lib')
    scripts_dir = os.path.join(project_dir, 'scripts')

    print('Project Directory: ' + project_dir)
    print('Asset Directory: ' + asset_dir)
    print('Library Directory: ' + lib_dir)
    print('Scripts Directory: ' + scripts_dir)

    print('Build Type: ' + build_type)
    print('Build Location: ' + build_location)

    if build_type != '32' and build_type != '64':
        sys.exit('invalid build type')

    copy_runtime()
    build_assets()
    copy_assets()
