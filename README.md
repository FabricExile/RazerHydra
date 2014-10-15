RazerHydra
==========
A KL extension for the Sixense Razer Hydra controller.

RazerHydra allows you to read the device tracking information as well as getting information about button states etc.

extension status
================

This extension has been built and tested against Fabric Engine 1.3.1. Higher release might work, but this hasn't been validated. Feel free to use this code for reference or training purposes, it's meant to be an example of how to integrate custom libraries into KL.

RazerHydra is currently only supported on Windows.

structure
=========

The RazerHydra extension uses the KL2EDK tool to generate the required C++ headers. Since the number of callbacks is quite small, they are handcrafted and don't use KL2EDK's advanced codegen functionality.

The latest KL2EDK documentation can be found here: http://documentation.fabricengine.com/FabricEngine/latest/HTML/ExtensionAuthoringGuide/kl2edk.html

supported platforms
===================

To date all three major platforms (windows, linux, osx) are supported, if you build the thirdparty dependencies for the corresponding platform.

building
========

A scons (http://www.scons.org/) build script is provided. RazerHydra depends on
* The Razer Hydra SDK 
* A static build of boost (1.55.0 or higher)

To inform scons where to find the Fabric Engine includes as well as the thirdparty libraries, you need to set the following environment variables:

* FABRIC_DIR: Should point to Fabric Engine's installation folder.
* SIXENSE_INCLUDE_DIR: Should point to the include folder of the RazerHydra SDK.
* SIXENSE_LIBRARY_DIR: Should point to the library folder of the RazerHydra SDK.
* BOOST_INCLUDE_DIR: Should point to Boost include folder.
* BOOST_LIBRARY_DIR: Should point to Boost library folder.

The temporary files will be built into the *build* folder, while the structured output files will be placed in the *stage* folder. Optionally you can define the FABRIC_EXTS_INSTALL_DIR environment variable, to which the extension will be installed.

Note: The fabric.sh exists purely for Fabric Engine internal use.

testing
=======

The extension repository also contains a series of unit tests, with which you can validate a successful build, or simply use them for reference on how to use the extension. to launch all unit tests simply use python like this:

    python test/test_all.py

license
==========

The license used for this extensions is the new BSD license. You can find it in the root folder of the repository.