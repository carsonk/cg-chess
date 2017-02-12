import glob
import os
import zipfile

if __name__ == '__main__':
    currentDirectory = os.getcwd()

    if not os.path.isdir('./tmp'):
        os.mkdir('./tmp')

    assetPackage = zipfile.ZipFile('./tmp/assets', mode='w', compression=zipfile.ZIP_DEFLATED)
    os.chdir('../assets')
    assetFiles = glob.glob('**', recursive=True)
    for filename in assetFiles:
        assetPackage.write(filename)
    assetPackage.close()

    os.chdir(currentDirectory)
