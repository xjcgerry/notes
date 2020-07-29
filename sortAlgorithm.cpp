//https://blog.csdn.net/kuaizi_sophia/article/details/87954222	
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

void bubbleSort(vector<int>& array) {
	for (int i = 0; i < array.size(); i++) {
		//设置标志位判断本轮循环是否发生了交换，如果标志位一直是false，说明本轮已经有序，不需要再循环了，跳出
		//每一次循环都把最大的数换到序列末尾
		bool isExchanged = false;
		for (int j = 0; j < array.size() - i - 1; j++) {
			if (array[j] > array[j + 1]) {
				int temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
				isExchanged = true;
			}
		}
		if (!isExchanged) {
			break;
		}
	}
}

void selectSort(vector<int>& array) {
	//搜索整个列表，找出最小项，若此项不为第1项，就与第1项交换位置
	for (int i = 0; i < array.size(); i++) {
		int minIndex = i;	//得到最小项的索引
		for (int j = i + 1; j < array.size(); j++) {
			if (array[minIndex] > array[j])
				minIndex = j;
		}
		if (minIndex != i)
			swap(array[minIndex], array[i]);
	}
}

void insertSort(vector<int>& array) {
	//将第一个元素看做有序序列，后续元素看做无序序列，依次将无序序列元素插入有序序列当中
	for (int i = 1; i < array.size(); i++) {
		if (array[i] < array[i - 1]) {
			int itemToinsert = array[i];
			int j = i - 1;
			while (j >= 0) {
				if (array[j] >= itemToinsert) {
					array[j + 1] = array[j];
					j--;
				}
				else break;
			}
			array[j + 1] = itemToinsert;
		}
	}
}

//选择一个增量序列，初始增量gap=length/2，后续元素依次为前一元素除2，直至gap=1；
//每轮以gap为步长，在列表上进行采样，将列表分为gap个小组，在每个小组内进行选择排序；
void shellSort(vector<int>& array) {
	int n = array.size();
	for (int gap = n / 2; gap >= 1; gap /= 2) {
		for (int i = gap; i < n; i++) {
			int itemToinsert = array[i];
			int j = i - gap;
			while (j >= 0 && array[j] >= itemToinsert) {
				array[j + gap] = array[j];
				j -= gap;
			}
			array[j + gap] = itemToinsert;
		}
	}
}

//合并两个有序序列，两序列分别为array的0到mid部分，和mid+1到末尾部分
void merge(vector<int>& array, vector<int>& copyArray, int left, int right) {
	int mid = (left + right) / 2;
	int i = left, j = mid + 1, k = 0;
	while (i <= mid && j <= right) {
		if (array[i] > array[j])
			copyArray[k++] = array[j++];
		else
			copyArray[k++] = array[i++];
	}
	while (i <= mid)
		copyArray[k++] = array[i++];
	while (j <= right)
		copyArray[k++] = array[j++];

	for (int i = left; i <= right; i++)	//array从left到right，copyArray从0到right-left
		array[i] = copyArray[i - left];
}

void mergeSortHelp(vector<int>& array, vector<int>& copyArray, int left, int right) {
	if (left < right) {
		int mid = (left + right) / 2;
		mergeSortHelp(array, copyArray, left, mid);
		mergeSortHelp(array, copyArray, mid + 1, right);
		merge(array, copyArray, left, right);
	}
}

void mergeSort(vector<int>& array) {
	vector<int> copyArray(array);
	mergeSortHelp(array, copyArray, 0, array.size() - 1);
}

//https://blog.csdn.net/kuaizi_sophia/article/details/87954222
void quickSortHelper(vector<int>& array, int left, int right) {
	if (left >= right) {
		return;
	}
	int i = left, j = right, base, temp;
	base = array[left];
	//当基准数选择最左边的数字时，那么就应该先从右边开始搜索；
	//当基准数选择最右边的数字时，那么就应该先从左边开始搜索。
	//不论是从小到大排序还是从大到小排序！
	while (i < j) {
		//基准是最左边数字，从右边开始搜索
		while (array[j] >= base && i < j)
			j--;
		while (array[i] <= base && i < j)
			i++;
		if (i < j) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
		}
	}
	array[left] = array[i];
	array[i] = base;
	quickSortHelper(array, left, i - 1);
	quickSortHelper(array, i + 1, right);
}

void quickSort(vector<int>& array) {
	quickSortHelper(array, 0, array.size() - 1);
}

//递归方式构建大根堆（len是array长度，index是第一个非叶子结点的下标）
/*void adjust(vector<int>& array, int len, int index) {
	int left = 2 * index + 1;	//index的左节点
	int right = 2 * index + 2;	//index的右节点

	int maxIdx = index;
	if (left < len && array[left] > array[maxIdx]) maxIdx = left;
	if (right < len && array[right] > array[maxIdx]) maxIdx = right;

	if (maxIdx != index) {
		swap(array[maxIdx], array[index]);
		adjust(array, len, maxIdx);
	}
}*/

void adjust(vector<int>& array, int len, int index) {
	int temp = array[index];
	int child = 2 * index+ 1;	//左孩子
	while (child < len) {		//没有孩子
		if (child + 1 < len && array[child] < array[child + 1]) {
			child++;	//较大孩子的下标
		}
		if (temp < array[child]) {
			array[index] = array[child];
			//没有将temp赋值给孩子是因为还要迭代子树
			//如果其孩子中有大的，会上移，temp还要继续下移
			index = child;
			child = 2 * index + 1;
		}
		else
			break;
	}
	array[index] = temp;
}

void heapSort(vector<int>& array) {
	//构建大根堆（从最后一个非叶子结点向上）
	int size = array.size();
	for (int i = size / 2 - 1; i >= 0; i--) {
		adjust(array, size, i);
	}

	for (int i = size - 1; i >= 1; i--) {
		swap(array[0], array[i]);
		adjust(array, i, 0);
	}
}

int main() {
	vector<int> array{9, 1, 5, 8, 3, 7, 4, 6, 2};
	//bubbleSort(array);
	//selectSort(array);
	//insertSort(array);
	//shellSort(array);
	//mergeSort(array);	//数组越界了，没检查出来
	//quickSort(array);
	heapSort(array);
	for (auto i : array)
		cout << i << ',';
	system("pause");
	return 0;
}
