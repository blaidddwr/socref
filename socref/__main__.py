from .block_factory import Block_Factory
from .project_model import Project_Model


def main():
    try:
        Block_Factory().load("Python","socref_python")
    except ModuleNotFoundError:
        print("oops!")

if __name__ == "__main__":
    main()
