# AWS Gems for Open 3D Engine
A repository containing AWS related objects for Open 3D Engine (O3DE) 

## Download and Register AWS gems for O3DE

### Clone the repository 

```shell
git clone https://github.com/aws/o3de-repo
```

For more details on setting up the engine, refer to [Setting up O3DE from GitHub](https://o3de.org/docs/welcome-guide/setup/setup-from-github/) in the documentation.

### Setting up AWS gems

Since the AWS O3DE repo can be cloned anywhere on your local computer, we just need to tell O3DE where to find the extra objects in this repo by registering them.
From the O3DE repo folder, you can register some or all AWS objects using the `o3de register` command.
Since these are all optional objects, we may not need or want all the objects.
If we want to register a particular object such as a single gem we would issue the following command:
```
scripts\o3de.bat register --gem-path <aws-o3de-repo>/Gems/<gem name>
```
Or you may want to register all the Gems.
Since this repo follows the [standard O3DE compound repo format](https://github.com/o3de/o3de/wiki/O3DE-Standard-repo-formats) all the AWS o3de-repo gems will be in the `<aws-o3de-repo>/Gems` path.
We can therefore register all the gems in the AWS gems path with one command:
```
scripts\o3de.bat register --all-gems-path <aws-o3de-repo>/Gems
```
This can be repeated for any object type (if they exist):
```
scripts\o3de.bat register --all-engines-path <aws-o3de-repo>/Engines
scripts\o3de.bat register --all-projects-path <aws-o3de-repo>/Projects
scripts\o3de.bat register --all-gems-path <aws-o3de-repo>/Gems
scripts\o3de.bat register --all-templates-path <aws-o3de-repo>/Templates
scripts\o3de.bat register --all-restricted-path <aws-o3de-repo>/Restricted
```
If we registered a gem, which is a piece of a project like a plugin, and we want to use that gem in our project we would only have to tell O3DE to enable that gem for our project by using the `o3de enable-gem` command:
```
scripts\o3de.bat enable-gem --gem-name <gem name> --project-name <project name>
```

For a complete tutorial on project configuration, see [Creating Projects Using the Command Line Interface](https://o3de.org/docs/welcome-guide/create/creating-projects-using-cli/) in the documentation.


## Security

See [CONTRIBUTING](CONTRIBUTING.md#security-issue-notifications) for more information.

## License

This project is licensed under the Apache-2.0 License.
