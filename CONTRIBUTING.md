# Contribution

We would love for you to contribute to __Socrates' Reference__ and help make it even better than it is today!  As a contributor, here are the guidelines we would like you to follow:

* [Submission Guidelines](#submit)

Here are a list of useful shortcut links to commonly used references:

* [Submission Types](#submit_type)

## <a name="submit"></a> Commit Message Guidelines

We have precise rules about how our git commit messages can be formatted. This leads to __more readable messages__ that are easy to follow when looking through the __project history__.

### Commit Message Format

Each commit message consists of a __header__, a __body__, and a __footer__. They are separated by a blank line.

```
<HEADER>
<BLANK LINE>
<BODY>
<BLANK LINE>
<FOOTER>
```

### Header

The header's format includes a __type__ and a __subject__. The total column length must be equal to or less than 50. The subject must use correct and clear grammar.

```
<type>: <subject>
```

### Body

The body consists of multiple paragraphs each separated by a blank line. Each paragraph describes a change in a file or logical group of files. All changes to all files must be described in the body. A paragraph can only describe a single change. If multiple changes were made in the same file or logical group of files then multiple paragraphs describing each change must exist in the git commit message.

A paragraph's format in a body includes a __type__, a __scope__, and a __description__.

The __scope__ must be something that easily identifies the file or programming object that has changed. For example Model::Foo for the foo model class in C++, README for the README markdown documentation, or docs/SOME_FEATURE for documentation within a directory. If a change involves renaming the words used for __scope__ then use the new name for the change's __scope__, referencing its old name in the description.

The columns of any line in a paragraph must be equal to or less than 72. Paragraphs can be on multiple lines as needed.

```
<type>(<scope>): <description>
<BLANK LINE>
<type>(<scope>): <description>
...
```

### Body Description

The __description__ of each paragraph in a body must use correct and clear grammar. It must answer the following questions:

* __WHAT__: What changed?
* __WHY__: Why has it changed?

If the why is self-evident it can be omitted.

### Footer

The __footer__ references the Github Issue ID associated with the commit. The first line references the issue ID. The second line and thereafter is the issue title. The columns of each issue title line must be less than or equal to 72. The issue title can be on multiple lines.

```
Github Issue #<Issue ID>
<Issue Title>
```

The initial commit of a repository is a special case with the following single line as its footer.

```
INITIAL COMMIT
```

### <a name="submit_type"></a> Type

The __type__ must be one of the following:

* __build__: Changes that affect the build system or external dependencies.
* __docs__: Changes in the documentation.
* __feature__: A code change that adds a new public feature.
* __intern__: A code change that adds a new private feature.
* __fix__: A code change that fixes a bug.
* __perf__: Changes that improve performance.
* __refactor__: A code change that neither fixes nor adds a feature.
* __style__: Changes that do not affect the meaning of the code (white space, formatting, etc).
* __test__: Adding new unit tests or correcting existing ones.

An exclamation is added to the end of the type if it is a __BREAKING CHANGE__. A __BREAKING CHANGE__ is only required in the header if the change breaks the existing public __API__. If a change breaks the internal __API__ it is only required in body paragraphs.

A plus sign is added to the end of the type if it is a __BUILD FIX__. A __BUILD FIX__ is a change which fixes a broken build.

Additional types can be added in the future as required by this project.

### Examples

The following illustrates a C++ example of a correct git commit message:

```
feature: Added blah feature.

feature(Model::Foo): Added lala method to implement blah feature.

intern!(Model::Bar): Added code in the moo method to use the new blah
feature in the foo model class. This breaks how the moo method behaves
and must be accounted for wherever it is used. The moo method is not a
public API method so it is not a public breaking change and the header
does not have an exclamation.

fix(Model::Bar): Fixed bug in the lala method. This bug was made visible
from the new blah feature. Fixed it by staring at my screen very hard
with an annoyed face.

docs(README): Added description of new blah feature.

Github Issue #9999
Add Blah Feature
```
