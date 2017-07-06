//max min
	double minv = 0.0, maxv = 0.0, range = 0.0;
	double* minp = &minv;
	double* maxp = &maxv;
	Mat	X1 = X.col(0).clone();//X.colRange(0, 1).clone();
	minMaxIdx(X1, minp, maxp);
	range = maxv - minv; //around 32.9369
						 //cout << minv << endl;

	Mat leftSet, rightSet;// , leftResult, rightResult;//left set; right set
	double splitPoint, InfoGain = 0.0;
	for (int i = 0; i < n_thresholds; i++)
	{
		InfoGain = 0.0;
		//srand((unsigned)time(NULL));
		double randNo = rand() / double(RAND_MAX);//double from 0 to 1
		double point = minv + range * randNo;

		//get 2 (left,right) sets, clear first
		int countX = 0, countY = 0;
		leftSet.release();
		rightSet.release();
		for (int x = 0; x < X.rows; x++)
		{
			double* data = X.ptr<double>(x);
			//cout << data[0]<<"---------------------"<< point << endl;
			if (data[0] <= point)
			{
				Mat L = X.row(x).clone();//from 0 to N: rowRange(x, x + 1)
				leftSet.push_back(L);
				//countX++;//leftSet.rows
			}
			else
			{
				Mat R = X.row(x).clone();//from 0 to N: rowRange(x, x + 1)
				rightSet.push_back(R);
				//countY++;
			}
		}//cout << leftSet << countX <<endl;


		if (InfoGain < getInfoGain(X, leftSet, rightSet))
		{
			InfoGain = getInfoGain(X, leftSet, rightSet);//, countX, countY);//leftSet.rows, rightSet.rows
			leftResult = leftSet;
			rightResult = rightSet;
			splitPoint = point;
		}

	}
