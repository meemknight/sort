#include <iostream>
#include <random>
#include <intrin.h>
#include <cctype>
#include <limits>
#include <type_traits>
#include "profilerLib.h"

constexpr bool useSIMD = true;			//vectorizare algoritm
constexpr int generatii = 10;			//(numar de etape al algoritmului)
constexpr int indiviziCount = 8 * 100;	//(dimensiunea populatiei) indiviziCount has to be multiple of 8
constexpr int keepFit = 8;				//(top x indivizi care vor trece la urmatoarea generatie ne schimbati)has to be, multiple of 8
constexpr int factorMutatie = 4;		//(sansa de mutatie per bit se calculeaza asa: (0.5 ^ factorMutatie)%


//limitele sunt definite in jurul punctului maxim.
struct FunctieGrad2
{
	float a, b, c;

	float operator()(float x)
	{
		return a * x * x + b * x + c;
	}

	inline float calculateMax()
	{
		float delta = b * b - 4 * a * c;
		return -delta / (4 * a);
	}

	std::pair<float, float> getLimits()
	{
		float mid = -b / (2 * a);
		return {mid-100.f, mid+100.f};
	}


}functieGrad2{-3, 2.1, -0.2};


using Individ = std::uint16_t; //(precizie), it can be uint8_t, uint16_t, uint32_t, uint64_t


static_assert(std::is_unsigned_v<Individ> &&std::is_integral_v<Individ>, "Indivizii trebuie sa fie de un tip unsigned int");




//todo mutatii
//todo alegere ponderata
int main()
{

	PL::Profiler profiler;


	std::mt19937 randomGenerator{std::random_device()()};
	std::uniform_int_distribution<Individ> dist;


	

	//todo make shure indivizi is multiple of 4 8 etc

	profiler.start();

	std::vector<Individ> indivizi(indiviziCount);
	std::generate(indivizi.begin(), indivizi.end(), [&]() { return dist(randomGenerator); });
	
	auto limits = functieGrad2.getLimits();

	//functia care returneaza valoarea float calculata la un individ

	auto getCromozonValue = [&](Individ &i) -> float
	{
		double d = i;
		d /= std::numeric_limits<Individ>::max();
		d *= limits.second - limits.first;
		d += limits.first;
		return (float)d;
	};



	for (int generatie = 0; generatie < generatii; generatie++)
	{
		
		//generare cromozomi noi;
		
		//keep some of the top individuals
		const int indiviziNoiCount = indiviziCount - keepFit;

		//random elements biased by fitness value

		std::random_shuffle(indivizi.begin(), indivizi.end());
		std::vector<Individ> parinte1(indivizi.begin(), indivizi.begin() + indiviziNoiCount);

		std::random_shuffle(indivizi.begin(), indivizi.end());
		std::vector<Individ> parinte2(indivizi.begin(), indivizi.begin()+ indiviziNoiCount);

		//bitii din masca ne spun daca pastram din indivitul 1 sau din individul 2
		std::vector<Individ> masca(indiviziNoiCount);
		std::generate(masca.begin(), masca.end(), [&]() { return dist(randomGenerator); });

		//bitul de 1 in mutatii inseamna o mutatie. 
		std::vector<Individ> mutatii(indiviziNoiCount);
		
		if constexpr (factorMutatie > 0)
		{
			std::generate(mutatii.begin(), mutatii.end(), [&]()
			{
				Individ m = std::numeric_limits<Individ>::max();

				for (int i = 0; i < factorMutatie; i++)
				{
					m &= dist(randomGenerator);
				}

				return m;
			});
		}

		//we move to front top fittest elements
		for (int i = 0; i < keepFit; i++)
		{
			float m = -1;
			int pos = 0;
			for (int j = i; j < indiviziCount; j++)
			{
				float val = functieGrad2(getCromozonValue(indivizi[j]));
				if (val > m)
				{
					m = val;
					pos = j;
				}
			}

			std::swap(indivizi[i], indivizi[pos]);
		}

		//incrucisare + mutatii
		if constexpr (std::is_same_v<Individ, std::uint16_t> && useSIMD)
		{
			//using 8 * 16 = 128

			for (int i = keepFit; i < indiviziCount; i += 8)
			{
				__m128i firstEl = _mm_and_si128(*(__m128i *) & masca[i - keepFit], *(__m128i *) & parinte1[i - keepFit]);
				__m128i secondEl = _mm_andnot_si128(*(__m128i *) & masca[i - keepFit], *(__m128i *) & parinte2[i - keepFit]);
				*(__m128i *) &indivizi[i] = _mm_or_si128(firstEl, secondEl);
				
				if constexpr (factorMutatie > 0)
					*(__m128i *) &indivizi[i] = _mm_xor_si128(*(__m128i *) & indivizi[i], *(__m128i *) & mutatii[i - keepFit]);

			}
		}
		else if constexpr (std::is_same_v<Individ, std::uint32_t> && useSIMD)
		{
			//using 4 * 32 = 128

			for (int i = keepFit; i < indiviziCount; i += 4)
			{
				__m128i firstEl = _mm_and_si128(*(__m128i *) & masca[i - keepFit], *(__m128i *) & parinte1[i - keepFit]);
				__m128i secondEl = _mm_andnot_si128(*(__m128i *) & masca[i - keepFit], *(__m128i *) & parinte2[i - keepFit]);
				*(__m128i *) &indivizi[i] = _mm_or_si128(firstEl, secondEl);
				
				if constexpr (factorMutatie > 0)
					*(__m128i *) &indivizi[i] = _mm_xor_si128(*(__m128i *) & indivizi[i], *(__m128i *) & mutatii[i - keepFit]);

			}
		}
		else if constexpr (std::is_same_v<Individ, std::uint64_t> && useSIMD)
		{
			//using 2 * 64 = 128

			for (int i = keepFit; i < indiviziCount; i += 2)
			{
				__m128i firstEl = _mm_and_si128(*(__m128i *) & masca[i - keepFit], *(__m128i *) & parinte1[i - keepFit]);
				__m128i secondEl = _mm_andnot_si128(*(__m128i *) & masca[i - keepFit], *(__m128i *) & parinte2[i - keepFit]);
				*(__m128i *) &indivizi[i] = _mm_or_si128(firstEl, secondEl);
				
				if constexpr (factorMutatie > 0)
					*(__m128i *) &indivizi[i] = _mm_xor_si128(*(__m128i *) & indivizi[i], *(__m128i *) & mutatii[i - keepFit]);
			}

		}
		else
		{
			for (int i = keepFit; i < indiviziCount; i += 4)
			{
				//loop unrolling can significatly improve the speed

				//the first line merges the 2 parents. we substract keepFit because the first keepFit elements are not changed.
				//the next 2 lines apply mutations using an xor because xor flips bits

			#define UNROLL(I) \
				indivizi[(I)] = (parinte1[(I)- keepFit] & masca[(I) - keepFit]) | (parinte2[(I) - keepFit] & (!masca[(I) - keepFit])); \
				if constexpr (factorMutatie > 0) \
					indivizi[(I)] = indivizi[(I)] ^ mutatii[(I) - keepFit]; \

				UNROLL(i + 0);
				UNROLL(i + 1);
				UNROLL(i + 2);
				UNROLL(i + 3);


			#undef UNROLL
			}
		}
			

	}

	auto rez = profiler.end();


	std::cout << "valoare estimata: " << functieGrad2(getCromozonValue(indivizi[0])) << "\n";
	std::cout << "valoare reala:    " << functieGrad2.calculateMax() << "\n";
	std::cout << "time secunde: " << rez.timeSeconds  << ",   cicli procesor: " << rez.cpuClocks << ".\n";


	std::cin.get();
	return 0;

}