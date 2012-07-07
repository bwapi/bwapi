from _winreg import *
from win32com.client import Dispatch
import filecmp
import os, shutil, time, winshell

def printOutput(s):
	print "["+time.strftime("%H:%M:%S", now)+"]",s

def getInstallPath(where):
	ret = ""
	aReg = ConnectRegistry(None,where)
	CreateKey(aReg, r"SOFTWARE\Blizzard Entertainment\Starcraft")
	aKey = OpenKey(aReg, r"SOFTWARE\Blizzard Entertainment\Starcraft")
	for i in range(1024):										   
		try:
			n,v,t = EnumValue(aKey,i)
			if n == "InstallPath":
				ret = v
				break
		except EnvironmentError:											   
			break  
	CloseKey(aKey)
	
	if ret != "" and os.path.exists(ret+"\\StarCraft.exe"):
		return ret
	else:
		return ""
		
	

installerPath = os.path.realpath(__file__).rpartition("\\")[0]
scPath = ""
chaosPath = installerPath+"\\Chaoslauncher"
now = time.localtime(time.time())

# Installer: Get Path
printOutput("Installer at "+installerPath)

# StarCraft: Get Install Path from Registry
scPath = getInstallPath(HKEY_CURRENT_USER)
if scPath == "": scPath = getInstallPath(HKEY_LOCAL_MACHINE)
if scPath == "":
	# If not found in registry, display an "open file" dialog
	import tkFileDialog, Tkinter
	root = Tkinter.Tk()
	root.withdraw()
	scPath = tkFileDialog.askopenfilename(filetypes=[("StarCraft","StarCraft.exe")],parent=root).rstrip("StarCraft.exe").rstrip("/").rstrip("\\")
if scPath == "" or not os.path.exists(scPath+"\\StarCraft.exe"):
	printOutput("ERROR: StarCraft Installation not found!\n           Install (or reinstall) the game please.")
	raw_input("Press Enter...")
	exit()

# Chaoslauncher: Get Path
printOutput("Chaoslauncher at "+chaosPath)

# Chaoslauncher: Write needed Registry Keys
printOutput("Modifying Chaoslauncher-related registry keys.")
bReg = ConnectRegistry(None,HKEY_CURRENT_USER)
CreateKey(bReg, r"SOFTWARE\Chaoslauncher\PluginsEnabled")
bKey = OpenKey(bReg, r"SOFTWARE\Chaoslauncher\PluginsEnabled", 0, KEY_WRITE)
try:   
   SetValueEx(bKey,"BWAPI Injector (1.16.1) DEBUG",0, REG_SZ, r"0") 
except EnvironmentError:                                          
    print "Encountered problems writing into the Registry..."
CloseKey(bKey)
bKey = OpenKey(bReg, r"SOFTWARE\Chaoslauncher\PluginsEnabled", 0, KEY_WRITE)
try:   
   SetValueEx(bKey,"BWAPI Injector (1.16.1) RELEASE",0, REG_SZ, r"1") 
except EnvironmentError:                                          
    print "Encountered problems writing into the Registry..."
CloseKey(bKey)
bKey = OpenKey(bReg, r"SOFTWARE\Chaoslauncher\PluginsEnabled", 0, KEY_WRITE)
try:   
   SetValueEx(bKey,"W-MODE 1.02",0, REG_SZ, r"1") 
except EnvironmentError:                                          
    print "Encountered problems writing into the Registry..."
CloseKey(bKey)

# Copy new BWAPI dlls somewhere in the PATH
bwapiDlls = []
for dirname, dirnames, filenames in os.walk('./WINDOWS'):
	for filename in filenames:
		bwapiDlls.append(filename)
oldPath = os.environ["PATH"].split(";")

alreadyInPath = ""
for folder in oldPath:
	for dll in bwapiDlls:
		try:
			if dll in os.listdir(folder):
				print time.strftime("%H:%M:%S", now),"",dll,"was found in",folder+". Copying new versions of required DLLs there."
				alreadyInPath = folder
				break
		except:
			continue

if alreadyInPath != "":
	# If our DLLs are already in PATH, just replace them with new versions
	for dll in bwapiDlls:
		shutil.copy2("./WINDOWS/"+dll,alreadyInPath)
else:
	# Otherwise, add them somewhere in "WINDOWS" install directory
	for folder in oldPath:
		if folder.find("WINDOWS") != -1 or folder.find("windows"):
			for dll in bwapiDlls:
				shutil.copy2("./WINDOWS/"+dll,folder)
			printOutput("Copying required DLLs to "+folder)
			break

# Copy everything needed to the StarCraft folder
if os.path.exists(scPath+"\\bwapi-data\\bwapi.ini") and not filecmp.cmp(scPath+"\\bwapi-data\\bwapi.ini",installerPath+"\\Starcraft\\bwapi-data\\bwapi.ini"): 
	printOutput("Creating a backup of bwapi.ini file.")
	shutil.move(scPath+"\\bwapi-data\\bwapi.ini",scPath+"\\bwapi-data\\bwapi.ini.bak")
root_src_dir = ".\\Starcraft"
root_dst_dir = scPath
for src_dir, dirs, files in os.walk(root_src_dir):
	dst_dir = src_dir.replace(root_src_dir, root_dst_dir)
	if not os.path.exists(dst_dir):
		printOutput("Creating directory "+dst_dir)
		os.mkdir(dst_dir)
	for file_ in files:
		src_file = os.path.join(src_dir, file_)
		dst_file = os.path.join(dst_dir, file_)
		if os.path.exists(dst_file):
			os.remove(dst_file)
			printOutput("Removing "+dst_file)
		shutil.copy(src_file, dst_dir)
		printOutput("Copying "+src_file+" to "+dst_file)

# Create a shortcut to new Chaoslauncher
printOutput("Creating a Desktop Icon to BWAPI-enabled Chaoslauncher.")
desktop = winshell.desktop()
path = os.path.join(desktop, "BWAPI.lnk")
target = r""+chaosPath+"\\Chaoslauncher - MultiInstance.exe"
wDir = r""+chaosPath
icon = r""+chaosPath+"\\Chaoslauncher - MultiInstance.exe"
shell = Dispatch('WScript.Shell')
shortcut = shell.CreateShortCut(path)
shortcut.Targetpath = target
shortcut.WorkingDirectory = wDir
shortcut.IconLocation = icon
shortcut.save()

# Open bwapi.ini in Notepad
os.system("start notepad "+scPath+"\\bwapi-data\\bwapi.ini")

# Done
print ""
printOutput("Finished! Don't forget to configure your bwapi.ini file.")
print ""
raw_input("Press Enter...")





