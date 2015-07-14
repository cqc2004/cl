#include "stdafx.h"
#include <stdio.h>
#include <vector>
#include <iterator>
#include <stack>
#include <cmath>
#include "chanlun.h"

using namespace std;

enum LineType {
	HIGH=1,LOW=2,NOTFIND=3
};

enum QueKou {
	LEFT, RIGHT, QUEKOUNOTFIND
};

typedef struct _kLine{
	float high = -1;
	float low = -1;
	int index;
	int left = -1;
	int right = -1;
	bool avaliable = false;
	float top = -1;
	float buttom = -1;
	QueKou leftquekou = QUEKOUNOTFIND;
	QueKou rightquekou = QUEKOUNOTFIND;
	LineType type;
}kLine;

void leftHighMerge(int dataLen, float* high, float* low, int index, kLine& pKLine){

	if (index == 0){
		return;
	}
	bool higherThanBefore = pKLine.high >= high[index - 1] ? true : false;
	bool smallThanBefore = pKLine.low <= low[index - 1] ? true : false;

	if (higherThanBefore && smallThanBefore){
		pKLine.low = low[index - 1];
		leftHighMerge(dataLen, high, low, index - 1, pKLine);
	}
	else if (!higherThanBefore){
		pKLine.avaliable = false;
	}
	else if (!smallThanBefore){
		pKLine.left = index - 1;
		if (pKLine.buttom == -1){
			pKLine.buttom = low[index - 1];
		}else if(pKLine.buttom > low[index -1]){
			pKLine.buttom = low[index - 1];
		}
		pKLine.avaliable = true;
	}
}

void rightHighMerge(int dataLen, float* high, float* low, int index, kLine& pKLine){
	if (index == dataLen - 1){
		return;
	}

	bool higherThanAfter = pKLine.high >= high[index + 1] ? true : false;
	bool smallThanAfter = pKLine.low <= low[index + 1] ? true : false;

	if (higherThanAfter && smallThanAfter){
		pKLine.low = low[index + 1];
		rightHighMerge(dataLen, high, low, index + 1, pKLine);
	}
	else if (!higherThanAfter){
		pKLine.avaliable = false;
	}
	else if (!smallThanAfter){
		pKLine.right = index + 1;
		if (pKLine.buttom == -1){
			pKLine.buttom = low[index + 1];
		}else if (pKLine.buttom > low[index + 1]){
			pKLine.buttom = low[index + 1];
		}
		pKLine.avaliable = true;
	}
}


void leftLowMerge(int dataLen, float* high, float* low, int index, kLine& pKLine){

	if (index == 0){
		return;
	}
	bool higherThanBefore = pKLine.high >= high[index - 1] ? true : false;
	bool smallThanBefore = pKLine.low <= low[index - 1] ? true : false;

	if (higherThanBefore && smallThanBefore){
		pKLine.high = high[index - 1];
		leftLowMerge(dataLen, high, low, index - 1, pKLine);
	}
	else if (!smallThanBefore){
		pKLine.avaliable = false;
	}
	else if (!higherThanBefore){
		pKLine.left = index - 1;
		if (pKLine.top == -1){
			pKLine.top = high[index - 1];
		}else if (pKLine.top < high[index-1]){
			pKLine.top = high[index - 1];
		}
		pKLine.avaliable = true;
	}
}

void rightLowMerge(int dataLen, float* high, float* low, int index, kLine& pKLine){
	if (index == dataLen - 1){
		return;
	}

	bool higherThanAfter = pKLine.high >= high[index + 1] ? true : false;
	bool smallThanAfter = pKLine.low <= low[index + 1] ? true : false;

	if (higherThanAfter && smallThanAfter){
		pKLine.high = high[index + 1];
		rightLowMerge(dataLen, high, low, index + 1, pKLine);
	}
	else if (!smallThanAfter){
		pKLine.avaliable = false;
	}
	else if (!higherThanAfter){
		pKLine.right = index + 1;
		if (pKLine.top == -1){
			pKLine.top = high[index + 1];
		}
		else if (pKLine.top < high[index+1]){
			pKLine.top = high[index + 1];
		}
		pKLine.avaliable = true;
	}
}

void fenxing(int dataLen, float* pOut, float* high, float* low, float* tday)
{
	vector<kLine> gvec;
	WCHAR wchar_pintf[1024];
	for (int i = 1; i < dataLen - 2; i++){
		bool higherThanBefore = high[i] >= high[i - 1] ? true : false;
		bool higherThanAfter = high[i] >= high[i + 1] ? true : false;
		
		bool smallThanBefore = low[i] <= low[i - 1] ? true : false;
		bool smallThanAfter = low[i] <= low[i + 1] ? true : false;
		
		if (higherThanBefore && higherThanAfter) {
			kLine kline;
			kline.high = high[i];
			kline.low = low[i];
			kline.index = i;
			kline.type = HIGH;
			kline.top = high[i];

			if (low[i] > high[i - 1] * 1.1) {
				kline.leftquekou = LEFT;
			}
			if (low[i] > high[i+1] * 1.1){
				kline.rightquekou = RIGHT;
			}
			if (kline.leftquekou==LEFT){
				kline.avaliable = true;
				kline.left = i;
				kline.buttom = low[i];
			}
			else{
				leftHighMerge(dataLen, high, low, i, kline);
			}
			
			if (kline.avaliable){
				if (kline.rightquekou=RIGHT){
					kline.avaliable = true;
					kline.right = i;
				}
				else{
					rightHighMerge(dataLen, high, low, i, kline);
				}
				
				if (kline.right!=-1){
					gvec.push_back(kline);
				}
			}
		}
		if (smallThanBefore && smallThanAfter){
			kLine kline;
			kline.high = high[i];
			kline.low = low[i];
			kline.index = i;
			kline.type = LOW;
			kline.buttom = low[i];

			if (high[i]*1.1 < low[i - 1]) {
				kline.leftquekou = LEFT;
			}
			if (high[i] * 1.1 < low[i + 1]){
				kline.rightquekou = RIGHT;
			}

			if (kline.leftquekou == LEFT){
				kline.avaliable = true;
				kline.left = i;
				kline.top = high[i];
			}
			else{
				leftLowMerge(dataLen, high, low, i, kline);
			}
			
			if (kline.avaliable){
				if (kline.rightquekou == RIGHT){
					kline.avaliable = true;
					kline.right = i;
				}
				else{
					rightLowMerge(dataLen, high, low, i, kline);
				}
				
				if (kline.right != -1){
					gvec.push_back(kline);
				}
			}
		}
	}
	
	memset(pOut, 0, dataLen);
	
	if (gvec.empty()){
		return;
	}
	vector<kLine>::iterator it = gvec.begin();
	LineType lineType = (*it).type;
	stack<kLine> kstack;
	kstack.push(*it);
	std::advance(it, 1);
	
	for (; it != gvec.end(); it++){
		kLine kline = kstack.top();
		kLine cur = *it;

		if (kline.type == HIGH){
			if (cur.type == HIGH){
				if (cur.high > kline.high){
					kstack.pop();
					kstack.push(cur);
				}
				continue;
			}

			if (kline.rightquekou==RIGHT || cur.leftquekou==LEFT){
				kstack.push(cur);
				lineType = cur.type;
				continue;
			}

			if (kline.right > cur.left || cur.index - kline.index<=3){
				continue;
			}

			float stop = kline.top > cur.top ? cur.top : kline.top;
		    float sbuttom = kline.buttom > cur.buttom ? kline.buttom : cur.buttom;

			float diff = stop - sbuttom;
			float ldiff = abs(diff) / (kline.top - kline.buttom);
			float rdiff = abs(diff) / (cur.top - cur.buttom);

			if ((diff <= 0 || (ldiff <= 0.2 && rdiff <= 0.2)) && kline.high > cur.high && kline.low > cur.low){
				kstack.push(cur);
				lineType = cur.type;
			}
		}else{
			if (cur.type == LOW){
				if (cur.low < kline.low){
					kstack.pop();
					kstack.push(cur);
				}
				continue;
			}
			
			if (kline.rightquekou == RIGHT || cur.leftquekou == LEFT){
				kstack.push(cur);
				lineType = cur.type;
				continue;
			}

			if (kline.right > cur.left || cur.index - kline.index<=3){
				continue;
			}
			

			float stop = kline.top > cur.top ? cur.top : kline.top;
			float sbuttom = kline.buttom > cur.buttom ? kline.buttom : cur.buttom;

			float diff = stop - sbuttom;
			float ldiff = abs(diff) / (kline.top - kline.buttom);
			float rdiff = abs(diff) / (cur.top - cur.buttom);

			if ((diff <= 0 || (ldiff <= 0.2 && rdiff <= 0.2)) && cur.high > kline.high && cur.low > kline.low){
				kstack.push(cur);
				lineType = cur.type;
			}

		}

	}
	
	while (!kstack.empty()){
		pOut[kstack.top().index] = kstack.top().type;
		kstack.pop();
	}

}

void fenxing_test(int dataLen, float* pOut, float* high, float* low, float* tday)
{
	vector<kLine> gvec;
	WCHAR wchar_pintf[1024];

	swprintf(wchar_pintf, 1024, L"Length: %d",dataLen);
	OutputDebugString(wchar_pintf);

	for (int i = 1; i < dataLen - 2; i++){
		bool higherThanBefore = high[i] >= high[i - 1] ? true : false;
		bool higherThanAfter = high[i] >= high[i + 1] ? true : false;

		bool smallThanBefore = low[i] <= low[i - 1] ? true : false;
		bool smallThanAfter = low[i] <= low[i + 1] ? true : false;

		if (higherThanBefore && higherThanAfter) {
			kLine kline;
			kline.high = high[i];
			kline.low = low[i];
			kline.index = i;
			kline.type = HIGH;
			kline.top = high[i];
			leftHighMerge(dataLen, high, low, i, kline);
			if (kline.avaliable){
				rightHighMerge(dataLen, high, low, i, kline);
				if (kline.right != -1){
					gvec.push_back(kline);
				}
			}
		}
		if (smallThanBefore && smallThanAfter){
			kLine kline;
			kline.high = high[i];
			kline.low = low[i];
			kline.index = i;
			kline.type = LOW;

			swprintf(wchar_pintf, 1024, L"DATE: %f LOW: %f  PRE: %f  NEXT: %f", tday[i], low[i], low[i-1],low[i+1]);
			OutputDebugString(wchar_pintf);

			leftLowMerge(dataLen, high, low, i, kline);
			if (kline.avaliable){
				rightLowMerge(dataLen, high, low, i, kline);
				if (kline.right != -1){
					gvec.push_back(kline);
				}
			}
		}
	}

	memset(pOut, 0, dataLen);

	if (gvec.empty()){
		return;
	}
	
	for (vector<kLine>::iterator it = gvec.begin(); it != gvec.end();it++){
		pOut[(*it).index] = (*it).type;
	}

}




//加载的函数
PluginTCalcFuncInfo g_CalcFuncSets[] =
{
	{ 1, (pPluginFUNC)&fenxing },
	{ 2, (pPluginFUNC)&fenxing_test },
	{ 0, NULL },
};

//导出给TCalc的注册函数
bool RegisterTdxFunc(PluginTCalcFuncInfo** pFun)
{
	if (*pFun == NULL)
	{
		(*pFun) = g_CalcFuncSets;
		return TRUE;
	}
	return FALSE;
}

