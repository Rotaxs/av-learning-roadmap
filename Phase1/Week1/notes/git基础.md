# Git 基础

## 

## git 基本指令

### `git add`

将工作区中修改或新建的文件追踪并添加到暂存区（Stage Area）

将制定文件放入暂存区

```bash
git add <file>
```

将当前目录及子目录所有文件防区暂存区

```bash
git add .
```

### `git commit`

将暂存区中的内容保存一个永久的代码快照，并生成唯一的 SHA-1 哈希值

带信息的直接提交

```bash
git commit -m "提交信息"
```

修改最近一次提交记录

```bash
git commit --amend
```

### `git status`

用于查看工作区和暂存区的实时状态

```bash
╰─➤  git status
On branch main
Your branch is up to date with 'origin/main'.

Changes to be committed:
  (use "git restore --staged <file>..." to unstage)
        new file:   "Phase1/Week1/notes/git\345\237\272\347\241\200.md"

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
        modified:   "Phase1/Week1/notes/git\345\237\272\347\241\200.md"
        modified:   "Phase1/Week1/\345\255\246\344\271\240\350\277\233\345\272\246.md"

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        test.txt
```

显示信息：

- 当前分支状态，如当前在 `main` 分支
- 已暂存的变更 `Changes to be committed`
- 未暂存的变更 `Changes not staged for commit`
- 未被跟踪的文件 `Untracked files`

使用 `git status -s` 可以精简格式输出，文件名前会显示双字符状态码

- `M` 表示修改
- `??` 表示为跟踪
- 绿色表示已暂存，红色表示未暂存

### `git diff`

查看未暂存的修改，被追踪的文件中未 `add` 的部分

```bash
git diff
```

查看已暂存的修改，`add` 了但是没有 `commit` 的部分

```bash
git diff --staged
```

查看当前工作区所有改动（包含以暂存和未暂存）

```bash
git diff HEAD
```

仅对比某一个文件

```bash
git diff path/to/file.txt
```

比较两个分支的差异

```bash
git diff <branch1> <branch2>
```

参数 

- `-stat` 不显示具体代码行，仅显示文件修改统计
- `--name-only` 仅输出有差异的文件路径列表
- `-w` 忽略所有空格和缩进差异

### `git log`

查看项目的提交历史记录

默认按时间倒序展示完整的历史，包括**完整的 `Commit Hash`**，**作者**，**提交日期**和**提交信息**

```bash
git log
```

精简信息，每条提交只显示单行（短 Hash + 提交信息）

```bash
git log --oneline
```

限制输出数量

```bash
git log -n <N>
```

常用的组合方式

```bash
git log --graph --oneline --all
```

显示每次提交影响的文件列表以及增删行数统计

```bash
git log --stat
```

### `git show`

查看某次提交的完整改动（作者、时间、`commit message`、以及具体的增删改）

查看最近一次提交（`HEAD`）的信息

```bash
git show
```

查看指定 Commit 的信息

```bash
git show <Commit Hash>
```

查看某次提交中的指定文件

```bash
git show <Commit Hash> -- path/to/file
```

