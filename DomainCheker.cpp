#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <set>
#include <random>
#include <sstream>

using namespace std;



/// <summary>
///  ласс содержащий им€ интернет домена
/// »м€ содержитьс€ в виде реверсивной строки дл€ ускорени€ поиска и сравнени€
/// </summary>
class Domain {
public:

    Domain() {}

    /// <summary>
    /// —оздание нового домена
    /// </summary>
    /// <param name="domain">домен в виде строки. например "yandex.ru"</param>
    Domain(std::string domain) {
        domain_ = domain;
        std::reverse(domain_.begin(), domain_.end());
    }


    /// <summary>
    /// ѕроверка - €вл€етс€ ли домен, передаваемый в качестве аргумента поддоменом текущего
    /// </summary>
    /// <param name="subdomain">провер€емый одмен</param>
    /// <returns>истина, если €вл€етс€ поддоменом</returns>
    bool IsSubdomain(const Domain& subdomain) const {

        string domain, sub_d;
        if (domain_.back() != '.') domain = domain_ + '.';
        else domain = domain_;
        if (subdomain.domain_.back() != '.') sub_d = subdomain.domain_ + '.';
        else sub_d = subdomain.domain_;

        if (domain == sub_d.substr(0, domain.size())) {
            return true;
        }
        return false;

    }

    bool operator==(Domain& d) {
        return this->domain_ == d.domain_;

    }


    const std::string GetDomain() const {
        return domain_;
    }





private:

    std::string domain_;


};


/// <summary>
/// —охран€ет список запрещенных доменов и провер€ет входит ли домен в список запрещенных
/// </summary>
class DomainChecker {
public:


    /// <summary>
    ///  онструктор принимает список запрещенных доменов
    /// </summary>
    /// <typeparam name="Iterator">Iterator на контейнер объектов класса Domain</typeparam>
    /// <param name="first">итератор на начало списка</param>
    /// <param name="last">итератор на конец списка</param>
    template <typename Iterator>
    DomainChecker(Iterator first, Iterator last) {
        domains_.reserve(std::distance(first, last));
        Iterator current = first;
        while (current != last) {
            domains_.push_back(*current++);
        }
        std::sort(domains_.begin(), domains_.end(), [](const Domain& d1, const Domain& d2) {
            return d1.GetDomain() < d2.GetDomain();
            });

        auto it = std::unique(domains_.begin(), domains_.end(), [](const Domain& d1, const Domain& d2) {
            return d1.GetDomain() == d2.GetDomain() || d1.IsSubdomain(d2);
            });
        domains_.resize(std::distance(domains_.begin(), it));



    }

    /// <summary>
    /// ѕроверка - запрещен ли домен, переданный в качестве аргумента
    /// </summary>
    /// <param name="domain">объект класса Domain</param>
    /// <returns>возвращает истину, если домен запрещен</returns>
    bool IsForbidden(const Domain& domain) const {
        auto it = upper_bound(domains_.begin(), domains_.end(), domain,
            [](const Domain& d1, const Domain& d2) {
                return d1.GetDomain() < d2.GetDomain();
            }
        );
        if (it != domains_.begin()) {
            it--;
            if (it->GetDomain() == domain.GetDomain()) {
                return true;
            }
            if (it->IsSubdomain(domain)) {
                return true;
            }
        }
        if (domains_.size() == 1) {
            if (it->GetDomain() == domain.GetDomain()) {
                return true;
            }
            if (it->IsSubdomain(domain)) {
                return true;
            }
        }
        return false;
    }


private:


    std::vector<Domain> domains_;
};



/// <summary>
/// „итает заданное количество доменов из стандартного входа
/// </summary>
/// <param name="in">¬ход€щий поток</param>
/// <param name="domain_count">количество считываемых доменов</param>
/// <returns>возвращает вектор доменов</returns>
std::vector<Domain> ReadDomains(istream& in, int domain_count) {
    std::vector<Domain> result;

    for (int i = 0; i < domain_count; i++) {
        std::string domain;
        getline(in, domain);
        result.push_back(Domain(domain));
    }
    return result;
}


template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}


string GenerateWord(mt19937& generator, int max_length) {
    const int length = uniform_int_distribution(1, max_length)(generator);

    string word;
    word.reserve(length);
    for (int i = 0; i < length; ++i) {
        const int dot = uniform_int_distribution(0, 1)(generator);
        if ((i % 2) && dot) {
            word.push_back('.');
        }
        else {
            word.push_back(static_cast<char>(uniform_int_distribution<uint16_t>((uint16_t)'a' & 0xff, (uint16_t)'a' & 0xff)(generator) & 0xff));
        }
    }
    return word;
}



int main() {


    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}