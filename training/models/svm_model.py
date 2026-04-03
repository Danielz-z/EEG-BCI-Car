from sklearn.svm import SVC


def build_svm():
    return SVC(kernel="linear", probability=True)