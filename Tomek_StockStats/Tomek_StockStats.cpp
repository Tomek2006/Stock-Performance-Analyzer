// Tomek_StockStats.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
Coder: Tomek Marek
Purpose of program:Analyze stock performance over a given time period, calculates
min, max prices, volume, mean, median, standard deviation, and best and worst 5-day and 20-day performance 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <locale>

using namespace std;

struct StockStats
{
	string date;
	double open;
	double high;
	double low;
	double close;
	long long volume;

	StockStats (string d, double o, double h, double l, double c, long long v)
		: date(d), open(o), high(h), low(l), close(c), volume(v) {
	}


};

/**
 * Function: partition
 * Purpose: Partitions the array for QuickSort
 * Accepts: Vector of StockStats, low, high.
 * Returns: Partition index.
 */
int partition(vector<StockStats>& A, int low, int high) {
	double pivotValue = (A[low].open + A[low].close) / 2.0;
	low = low - 1; 
	high = high + 1; 

	while (true) {
		
		do {
			low = low + 1;
		} while ((A[low].open + A[low].close) /2.0 > pivotValue);

		
		do {
			high = high - 1;
		} while ((A[high].open + A[high].close) / 2.0 < pivotValue);

		
		if (low < high) {
			swap(A[low], A[high]);
		}
		else {
			return high; 
		}
	}
}

/**
 * Function: quickSort
 * Purpose: Sorts the array using QuickSort
 * Accepts: Vector of StockStats, low, high.
 * Returns: None.
 */
void quickSort(vector<StockStats>& A, int low, int high) {
	if (low < high) {
		int pivot = partition(A, low, high);
		quickSort(A, low, pivot);
		quickSort(A, pivot + 1, high);
	}
}


/**
 * Function: calculateMedian
 * Purpose: Calculates the median of the stock data
 * Accepts: Vector of StockStats.
 * Returns: Median.
 */
double calculateMedian(vector<StockStats> stockData) {
	quickSort(stockData, 0, stockData.size() - 1);

	if (stockData.size() % 2 == 0) {
		return (
			((stockData[stockData.size() / 2 - 1].open + stockData[stockData.size() / 2 - 1].close) / 2.0) +
			((stockData[stockData.size() / 2].open + stockData[stockData.size() / 2].close) / 2.0)
			) / 2.0;
	}
	else {
		return (stockData[stockData.size() / 2].open + stockData[stockData.size() / 2].close) / 2.0;
	}
}

/**
 * Function: calculateMean
 * Purpose: Calculates the mean of the stock data
 * Accepts: Vector of StockStats.
 * Returns: Mean.
 */
double calculateMean(vector<StockStats>& stockData) {
	double sum = 0.0;
	for (const auto& record : stockData) {
		sum += record.close;
	}
	return sum / stockData.size();
}

/**
 * Function: calculateStandardDeviation
 * Purpose: Calculates the standard deviation of the stock data
 * Accepts: Vector of StockStats, meanVolume.
 * Returns: Standard deviation.
 */
double calculateStandardDeviation(const vector<StockStats>& stockData, double meanVolume) {
	double sum = 0.0;  

	for (const auto& record : stockData) {
		sum += pow(record.volume - meanVolume, 2);
	}

	return sqrt(sum / stockData.size());  
}


/**
 * Function: calculateMeanVolume
 * Purpose: Calculates the mean volume of the stock data
 * Accepts: Vector of StockStats.
 * Returns: Mean volume.
 */
long long calculateMeanVolume(const vector<StockStats>& stockData) {
	long long sum = 0;
	for (const auto& record : stockData) {
		sum += record.volume;
	}
	return sum / stockData.size();
}



int main(int argc, char* argv[]) {
	if (argc != 2) {  
		cerr << "Error: No input file specified.\n"; 
		cerr << "Usage: " << argv[0] << " <csvFilename>\n";
		return 1;
	}

	string filename = argv[1]; 


	ifstream file(filename);
	if (!file.is_open()) {
		cerr << "Error: Cannot open file " << filename << endl;
		return 1;
	}
	string  line;
	getline(file, line); 


	vector<StockStats> stockData;


	while (getline(file, line)) {  
		stringstream ss(line);
		string date, openStr, highStr, lowStr, closeStr, volumeStr;

		getline(ss, date, ',');        
		getline(ss, openStr, ',');     
		getline(ss, highStr, ',');      
		getline(ss, lowStr, ',');      
		getline(ss, closeStr, ',');   
		getline(ss, volumeStr, ','); 


		double open = stod(openStr);    
		double high = stod(highStr);    
		double low = stod(lowStr);     
		double close = stod(closeStr);  
		long long volume = stoll(volumeStr); 

		stockData.emplace_back(date.substr(0, 10), open, high, low, close, volume);
	}


	file.close(); 

	
	string stockSymbol = filename.substr(0, filename.find('.'));
	string startDate = stockData.front().date;  
	string endDate = stockData.back().date;     

	
	cout.imbue(locale("en_US.UTF-8"));//Format numbers with commas

	//Stock summary
	cout << "stockstats (2025.0.0), (c) 2025 Tomek Marek\n";
	cout << "Stock: \"" << stockSymbol << "\"\n";
	cout << "From: " << startDate << " to " << endDate << "\n";
	cout << "# Days: " << stockData.size() << "\n\n";

	
	//Calculate and print the statistics
	double minPrice = stockData[0].low;
	string minPriceDate = stockData[0].date;

	double maxPrice = stockData[0].high;
	string maxPriceDate = stockData[0].date;

	long long minVolume = stockData[0].volume;
	string minVolumeDate = stockData[0].date;

	long long maxVolume = stockData[0].volume;
	string maxVolumeDate = stockData[0].date;


	for (const auto& record : stockData) {
		if (record.low < minPrice) {
			minPrice = record.low;
			minPriceDate = record.date;
		}

		if (record.high > maxPrice) {
			maxPrice = record.high;
			maxPriceDate = record.date;
		}

		if (record.volume < minVolume) {
			minVolume = record.volume;
			minVolumeDate = record.date;
		}

		if (record.volume > maxVolume) {
			maxVolume = record.volume;
			maxVolumeDate = record.date;
		}
	}

	cout << "\nHistorical\n";
	cout << "      Low price:"<< fixed<<setprecision(2) << setw(13) << minPrice << " on " << minPriceDate << endl;
	cout << "     High price:"<<fixed << setprecision(2) <<  setw(13) << maxPrice << " on " << maxPriceDate << endl;
	cout << "     Low volume:" << setw(13) << minVolume << " on " << minVolumeDate << endl;
	cout << "    High volume:" << setw(13) << maxVolume << " on " << maxVolumeDate << endl;

	double meanClose = calculateMean(stockData);
	double meanVolume = calculateMeanVolume(stockData);
	double stdDev = calculateStandardDeviation(stockData, meanVolume);
	double medianClose = calculateMedian(stockData);

	cout << "     Mean close:" << setw(13) << fixed << setprecision(2) << meanClose << endl;
	cout << "    Mean volume:" << setw(13) << fixed << setprecision(1) << meanVolume << endl;
	cout << "  Volume stddev:" << setw(13) << fixed << setprecision(1) << stdDev << endl;
	cout << "Median stock price:" << setw(13) << fixed << setprecision(1) << medianClose << endl;


	//Find best and worst 5-day and 20-day performance
	size_t best5Index{ 0 }, worst5Index{ 0 }, best20Index{ 0 }, worst20Index{ 0 };

	double best5Day{ stockData[0].open - stockData[4].close };
	double worst5Day{ stockData[0].open - stockData[4].close };
	double best20Day{ stockData[0].open - stockData[19].close };
	double worst20Day{ stockData[0].open - stockData[19].close };

	double fiveDayTrend, twentyDayTrend;

	for (size_t i = 1 ; i < stockData.size() - 5; ++i) {
		if (i < stockData.size() - 20) {
			twentyDayTrend = stockData[i + 19].close - stockData[i].open;
			if (twentyDayTrend > best20Day) {
				best20Index = i;
				best20Day = twentyDayTrend;
			}
			if (twentyDayTrend < worst20Day) {
				worst20Index = i;
				worst20Day = twentyDayTrend;
			}
		}
		fiveDayTrend = stockData[i + 4].close - stockData[i].open;
		if (fiveDayTrend > best5Day) {
			best5Index = i;
			best5Day = fiveDayTrend;
		}
		if (fiveDayTrend < worst5Day) {
			worst5Index = i;
			worst5Day = fiveDayTrend;
		}
	}

	
	cout << "\nWorst  5 day period: " << stockData[worst5Index].date << " to " << stockData[worst5Index + 4].date
		<< " =" << setw(12) << fixed << setprecision(1) << worst5Day << endl;

	cout << "Best   5 day period: " << stockData[best5Index].date << " to " << stockData[best5Index + 4].date
		<< " =" << setw(12) << fixed << setprecision(1) << best5Day << endl;

	cout << "\nWorst 20 day period: " << stockData[worst20Index].date << " to " << stockData[worst20Index + 19].date
		<< " =" << setw(13) << fixed << setprecision(1) << worst20Day << endl;

	cout << "Best  20 day period: " << stockData[best20Index].date << " to " << stockData[best20Index + 19].date
		<< " =" << setw(13) << fixed << setprecision(1) << best20Day << endl;


	return 0;
}