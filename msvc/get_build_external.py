from urllib.request import urlopen
from io import BytesIO
import zipfile
import subprocess
import platform

def download_unpack(url):
    """Download and unpack from url"""
    remotedata = urlopen(url)

    #Convert the downloaded data to an in memory file-like object
    zipdata = BytesIO()
    zipdata.write(remotedata.read())

    #Create a zipfile and extract it
    myzipfile = zipfile.ZipFile(zipdata)
    myzipfile.extractall()

def download_all():
    """Download and unpack all external libraries"""

    urls = ["http://bitbucket.org/eigen/eigen/get/3.3.0.zip",
            "https://github.com/zeromq/libzmq/releases/download/v4.2.2/zeromq-4.2.2.zip",
            "https://github.com/zeromq/cppzmq/archive/v4.2.2.zip",
           ]
    for url in urls:
        download_unpack(url)

def build_all():
    if platform.architecture()[0] == '64bit':
        subprocess.run("build_deps_amd64.bat")
    else:
        subprocess.run("build_deps_x86.bat")

if __name__ == "__main__":
    download_all()
    build_all()