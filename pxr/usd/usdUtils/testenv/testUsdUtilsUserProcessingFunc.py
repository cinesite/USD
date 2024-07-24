#!/pxrpythonsubst
#
# Copyright 2023 Pixar
#
# Licensed under the terms set forth in the LICENSE.txt file available at
# https://openusd.org/license.

from pxr import Ar, Sdf, Tf, Usd, UsdUtils
import os, sys, shutil, tempfile, unittest

class TestUsdUtilsUserProcessFunc(unittest.TestCase):
    def test_TransientDependencies(self):
        """Tests that transient dependencies that result from invocation of the
        user processing function are included in the package"""

        def TestUserFunc(layer, depInfo):
            validPath = depInfo.assetPath.replace('test_', '')
            return UsdUtils.DependencyInfo(validPath)

        rootPath = 'transientDeps/root.usda'
        localizationDir = 'transientDeps_localized'
        localizedRoot = os.path.join(localizationDir, 'root.usda')
        self._Localize(rootPath, localizationDir, TestUserFunc)
        self.assertIsNotNone(Usd.Stage.Open(localizedRoot))

        expectedFiles = [
            'dep_metadata.usda',
            'dep_payload.usda',
            'dep_prop.usda',
            'dep_prop_metadata.usda',
            'dep_reference.usda',
            'dep_sublayer.usda',
            'dep_timesample.usda',
            'metadata.usda',
            'payload.usda',
            'prop.usda',
            'prop_metadata.usda',
            'reference.usda',
            'root.usda',
            'sublayer.usda',
            'timesample.usda'
        ]

        self._CheckLocalizedPackageContents(localizationDir, expectedFiles)

    def test_TransientDependenciesTemplateAssetPath(self):
        """Tests that transient dependencies of clips template asset paths
        that are modified by user processing functions are included"""

        def TestUserFunc(layer, depInfo):
            validPath = depInfo.assetPath.replace('test_', '')
            validDeps = [x.replace('test_', '') for x in depInfo.dependencies]
            return UsdUtils.DependencyInfo(validPath, validDeps)
        
        rootPath = 'transientDepsTemplateAssetPath/root.usda'
        localizationDir = 'transientDepsTemplateAssetPath_localized'
        localizedRoot = os.path.join(localizationDir, 'root.usda')

        self._Localize(rootPath, localizationDir, TestUserFunc)
        self.assertIsNotNone(Usd.Stage.Open(localizedRoot))

        expectedFiles = [
            'clip.001.usda',
            'dep_clip.001.usda',
            'root.usda'
        ]

        self._CheckLocalizedPackageContents(localizationDir, expectedFiles)

    def test_UserProcessingFuncRemove(self):
        """Tests that dependencies can be removed with the user 
        processing func"""
        def TestUserFunc(layer, depInfo):
            return UsdUtils.DependencyInfo()

        rootPath = 'transientDeps/root.usda'
        localizationDir = 'transientDeps_remove_localized'
        localizedRoot = os.path.join(localizationDir, 'root.usda')

        self._Localize(rootPath, localizationDir, TestUserFunc)
        self.assertIsNotNone(Usd.Stage.Open(localizedRoot))

        expectedFiles = [
            'root.usda'
        ]

        self._CheckLocalizedPackageContents(localizationDir, expectedFiles)

    def test_FileFormatConversion(self):
        """Tests converting a layer to a crate file in a temp path and
        ensuring the resulting localized package can be loaded correctly"""
        
        with tempfile.TemporaryDirectory() as tempDir:
            def TestUserFunc(layer, depInfo):
                root, ext = os.path.splitext(depInfo.assetPath)
                if ext != '.usda':
                    return depInfo
                
                sourceDepLayer = Sdf.Layer.FindOrOpenRelativeToLayer(
                    layer, depInfo.assetPath)
                crateFileName = root + '.usdc'
                cratePath = os.path.join(tempDir, crateFileName)
                sourceDepLayer.Export(cratePath)
                return UsdUtils.DependencyInfo(cratePath)
        
            rootPath = 'fileFormatConversion/root.usda'
            localizationDir = 'fileFormatConversion_localized'
            localizedRoot = os.path.join(localizationDir, 'root.usda')

            self._Localize(rootPath, localizationDir, TestUserFunc)
            self.assertIsNotNone(Usd.Stage.Open(localizedRoot))

            expectedFiles = [
                '0/sublayer.usdc',
                'root.usda'
            ]

            self._CheckLocalizedPackageContents(localizationDir, expectedFiles)

    def test_CachedProcessingFuncValues(self):
        """Tests that the system caches processed asset path values and only
           invokes the callback once for each layer / path pair"""
        
        processedPaths = set()
        def TestUserFunc(layer, depInfo):
            self.assertTrue(depInfo.assetPath not in processedPaths)
            processedPaths.add(depInfo.assetPath)

            name, _ = os.path.splitext(os.path.basename(depInfo.assetPath))

            if name.startswith('modify'):
                return UsdUtils.DependencyInfo('./modified.usda')
            elif name.startswith('remove'):
                return UsdUtils.DependencyInfo()
            else:
                return depInfo
            
        rootPath = 'duplicatePaths/root.usda'
        localizationDir = 'duplicatePaths_localized'
        localizedRoot = os.path.join(localizationDir, 'root.usda')

        self._Localize(rootPath, localizationDir, TestUserFunc)
        self.assertIsNotNone(Usd.Stage.Open(localizedRoot))

        self.assertSetEqual(processedPaths, 
            {'./default.usda', './modify.usda', './remove.usda'})

        expectedFiles = [
            'default.usda',
            'modified.usda',
            'root.usda'
        ]

        self._CheckLocalizedPackageContents(localizationDir, expectedFiles)

    def test_FilesystemAbsolutePaths(self):
        """Tests that localization works correctly with absolute filesystem 
        paths"""

        def TestUserFunc(layer, depInfo):
            absPath = os.path.abspath(Sdf.ComputeAssetPathRelativeToLayer(
                layer, depInfo.assetPath))
            return UsdUtils.DependencyInfo(absPath)
    
        rootPath = os.path.abspath('fileFormatConversion/root.usda')
        localizationDir = os.path.abspath('abspath_localized')
        localizedRoot = os.path.join(localizationDir, 'root.usda')

        self._Localize(rootPath, localizationDir, TestUserFunc)
        self.assertIsNotNone(Usd.Stage.Open(localizedRoot))

        expectedFiles = [
            '0/sublayer.usda',
            'root.usda'
        ]

        self._CheckLocalizedPackageContents(localizationDir, expectedFiles)

    def test_CodingErrorPackageLayer(self):
        """Tests that a coding error is issued when trying to modify an asset
        path in a layer that is a package"""
        def ModifyPackageLayer(layer, depInfo):
            if layer.GetFileFormat().IsPackage():
                return UsdUtils.DependencyInfo("foo")
            else:
                return depInfo

        with self.assertRaises(Tf.ErrorException):
            self._Localize("package/package.usdz", "package_localized1", 
                           ModifyPackageLayer)
            
    def test_CodingErrorLayerContainedInPackage(self):
        """Tests that a coding error is issued when trying to modify an asset
        path of a layer that is contained in a package"""
        def ModifyLayerInPackage(layer, depInfo):
            if Ar.IsPackageRelativePath(layer.identifier):
                return UsdUtils.DependencyInfo("foo")
            else:
                return depInfo

        with self.assertRaises(Tf.ErrorException):
            self._Localize("package/package.usdz", "package_localized2", 
                           ModifyLayerInPackage)

    def _Localize(self, rootPath, localizationDir, userFunc):
        if os.path.isdir(localizationDir):
            shutil.rmtree(localizationDir)
        
        result = UsdUtils.LocalizeAsset(
            rootPath, localizationDir, processingFunc=userFunc)
        
        self.assertTrue(result)

    
    def _CheckLocalizedPackageContents(self, packagePath, expectedFiles):
        filesInArchive = self._GetFileList(packagePath)
        self.assertEqual(expectedFiles, filesInArchive)

    def _GetFileList(self, localizedAssetDir):
        rootFolderPathStr = localizedAssetDir + os.sep
        contents = []
        for path, directories, files in os.walk(localizedAssetDir):
            for file in files:
                localizedPath = os.path.join(path,file)
                localizedPath = localizedPath.replace(rootFolderPathStr, '')
                localizedPath = localizedPath.replace('\\', '/')
                contents.append(localizedPath)
        
        contents.sort()

        return contents


if __name__=="__main__":
    unittest.main()
