import os, fnmatch


def find_files(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for basename in files:
            if fnmatch.fnmatch(basename, pattern):
                filename = os.path.join(root, basename)
                yield filename






#for filename in find_files('/Users/admin/Desktop/Visual Renamer.app', '*.dylib'):
#    os.system("codesign -s \"3rd Party Mac Developer Application: Vincent Randazzo (4TF52UUX35)\" \"" + filename + "\"")
#    print 'Found:', filename

#os.system("codesign -s \"3rd Party Mac Developer Application: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtCore.framework")
#os.system("codesign -s \"3rd Party Mac Developer Application: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtGui.framework")
#os.system("codesign -s \"3rd Party Mac Developer Application: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtPrintSupport.framework")
#os.system("codesign -s \"3rd Party Mac Developer Application: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtWidgets.framework")

#os.system("codesign --force --verify --verbose --sign \"3rd Party Mac Developer Application: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtCore.framework/Versions/5/QtCore")
#os.system("codesign --force --verify --verbose --sign \"3rd Party Mac Developer Application: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtGui.framework/Versions/5/QtGui")
#os.system("codesign --force --verify --verbose --sign \"3rd Party Mac Developer Application: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport")
#os.system("codesign --force --verify --verbose --sign \"3rd Party Mac Developer Application: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets")

#os.system("codesign --sign \"3rd Party Mac Developer Application: Vincent Randazzo (4TF52UUX35)\" --entitlements Entitlements.plist --deep /Users/admin/Desktop/\'Visual Renamer.app\'")

os.system("productbuild --component  /Users/admin/Desktop/\"Visual Renamer.app\"  /Applications --sign \"3rd Party Mac Developer Installer: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.pkg\'")
