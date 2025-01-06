Here's the text with redundant statements eliminated within each paragraph:

# Umbra Gem for O3DE (Open 3D Engine)

The Umbra Gem is compatible with O3DE 24.0.9+.
 
The Umbra Gem integrates the Umbra SDK with O3DE. The gem interfaces with the O3DE editor and asset builder to export level geometry and bake occlusion culling data. The baked data is loaded and used in the editor or game client to optimize rendering performance by disregarding occluded or hidden objects. The following steps guide you through installing the Umbra Gem and SDK in O3DE, setting up projects and levels, exporting and baking data, and performing occlusion queries.
## Installation

### 1. Install the Umbra SDK

To use the Umbra Gem, you must obtain the Umbra SDK and a valid license from the [Umbra website](https://www.umbra3d.com). Unzip the SDK package into a local folder. If building from source, refer to the SDK instructions to build binaries for your target platforms. Targeting non standard or new platforms may require updating CMake in the gem to link the correct libraries.

### 2. Install the Umbra Gem

Download and copy the gem source to a local folder. Register and enable the gem with the O3DE project manager or command line interface.

The gem requires configuring the Umbra SDK path and license key. Set the SDK path using the UMBRA_SDK_PATH environment variable or CMake configuration (CMake takes precedence). Rebuild the project after setting the SDK path.

Set the Umbra license key to compute baked occlusion data using the asset builder. Place umbra_license.txt in the binaries folder, or set the UMBRA_LICENSE_KEY environment variable to the value stored in the file. A valid license is required to process scenes and bake data. The assets will not be processed and errors will be reported if the license is invalid. The license is not required for loading, using, or distributing existing scene data with the Umbra runtime. Set the UMBRA_SUPPRESS_LICENSE_DIALOG environment variable to TRUE to prevent the Umbra SDK optimizer from displaying a license-related warning dialog that may disrupt the asset processor.

## Using the Umbra Gem

The Umbra gem includes components to setup Umbra in O3DE.

### The Umbra Scene Component

The Umbra Scene Component must be added to the root entity of any level using Umbra occlusion culling. It has properties to configure what objects are exported with the scene, where the scene data is saved, how scene data is processed, and what debug data is visualized.

Use the component in the editor to export a level's scene geometry whenever occlusion data needs to be updated. By default, only geometry on static entities will be exported but the component has settings to configure this behavior. The asset processor uses the Umbra optimizer to compute visibility data from the exported scene geometry. The visibility data will be saved as an asset and automatically loaded for the Umbra runtime to determine object visibility during queries.

The scene export process gathers geometry from systems and components that implement AzFramework::VisibleGeometryRequestBus. Currently, only a few components, like the Mesh Component, implement this bus and contribute to the scene geometry. Other components, like shape and terrain components, can easily be extended with AzFramework::VisibleGeometryRequestBus to include their geometry. 

The Umbra Scene Component implements AzFramework::OcclusionRequestBus, which can be used to service occlusion queries and other requests throughout the engine. The Atom renderer will automatically perform per-view occlusion culling via this bus if the component is active and data is loaded.

### The Umbra Object Component

The Umbra Object Component is optional. Add it to entities with components, like the mesh component, that contribute to exported scene geometry. It can explicitly configure objects as occluders or occludees and overrides visibility computation settings for baking.

### The Umbra View Volume Component

The Umbra View Volume Component is used with shape components to define a valid camera movement area. View volumes help the Umbra optimizer compute visibility data from key areas in Umbra scenes. They have properties to override visibility settings within their defined area. Mark your level's playable volume with Umbra View Volumes to speed up computation and reduce generated data size.

### Umbra visibility data

The scene component's generated visibility data is saved in two files: the umbrascene file for the asset processor and Umbra optimizer to create runtime tome data, and the umbrascenedesc file mapping level entity IDs to Umbra scene object IDs. The asset processor uses both to produce the umbrasceneasset containing the tome visibility data and descriptor mapping.

## Additional Information

For further information on Umbra topics, see the documentation in the Umbra SDK release package or visit the [Umbra website](https://www.umbra3d.com).