import os, fnmatch


def find_files(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for basename in files:
            if fnmatch.fnmatch(basename, pattern):
                filename = os.path.join(root, basename)
                yield filename






#for filename in find_files('/Users/admin/Desktop', '*.dylib'):
#    os.system("codesign -s \"Developer ID Application: Vincent Randazzo (4TF52UUX35)\" \"" + filename + "\"")
#    print 'Found:', filename

#os.system("cp /Users/admin/Qt/5.3/clang_64/lib/QtCore.framework/Contents/Info.plist /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtCore.framework/Resources/")
#os.system("cp /Users/admin/Qt/5.3/clang_64/lib/QtGui.framework/Contents/Info.plist /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtGui.framework/Resources/")
#os.system("cp /Users/admin/Qt/5.3/clang_64/lib/QtPrintSupport.framework/Contents/Info.plist /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtPrintSupport.framework/Resources/")
#os.system("cp /Users/admin/Qt/5.3/clang_64/lib/QtWidgets.framework/Contents/Info.plist /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtWidgets.framework/Resources/")

#os.system("codesign -s \"Developer ID Application: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtCore.framework")
#os.system("codesign -s \"Developer ID Application: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtGui.framework")
#os.system("codesign -s \"Developer ID Application: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtPrintSupport.framework")
#os.system("codesign -s \"Developer ID Application: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.app\'/Contents/Frameworks/QtWidgets.framework")

#os.system("codesign -s \"Developer ID Application: Vincent Randazzo (4TF52UUX35)\" --entitlements Entitlements.plist --deep /Users/admin/Desktop/\'Visual Renamer.app\'")

os.system("productbuild --component  /Users/admin/Desktop/\"Visual Renamer.app\"  /Applications --sign \"Developer ID Installer: Vincent Randazzo (4TF52UUX35)\" /Users/admin/Desktop/\'Visual Renamer.pkg\'")
