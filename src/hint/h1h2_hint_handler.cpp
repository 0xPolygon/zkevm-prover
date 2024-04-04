#include "h1h2_hint_handler.hpp"

namespace Hints
{
    std::string H1H2HintHandler::getName()
    {
        return "h1h2";
    }

    std::vector<std::string> H1H2HintHandler::getSources() const
    {
        return {"f", "t"};
    }

    std::vector<std::string> H1H2HintHandler::getDestinations() const
    {
        return {"referenceH1", "referenceH2"};
    }

    size_t H1H2HintHandler::getMemoryNeeded(uint64_t N) const
    {
        return 8 * N * sizeof(Goldilocks::Element);
    }

    void H1H2HintHandler::resolveHint(int N, Hint hint, const std::map<std::string, Polinomial *> &polynomials, void *ptr_extra_mem) const
    {
        assert(polynomials.size() == 4);

        auto h1 = polynomials.find("referenceH1");
        auto h2 = polynomials.find("referenceH2");
        auto f = polynomials.find("f");
        auto t = polynomials.find("t");

        assert(h1 != polynomials.end());
        assert(h2 != polynomials.end());
        assert(f != polynomials.end());
        assert(t != polynomials.end());

        auto h1Pol = *h1->second;
        auto h2Pol = *h2->second;
        auto fPol = *f->second;
        auto tPol = *t->second;

        assert(h1Pol.dim() == 1 || h1Pol.dim() == 3);
        assert(h1Pol.dim() == h2Pol.dim());
        assert(h1Pol.dim() == fPol.dim());
        assert(h1Pol.dim() == tPol.dim());

        if (h1Pol.dim() == 1)
        {
            Polinomial::calculateH1H2_opt1(h1Pol, h2Pol, fPol, tPol, 0, (uint64_t *) ptr_extra_mem, 5 * N);
        }
        else if (h1Pol.dim() == 3)
        {
            Polinomial::calculateH1H2_opt3(h1Pol, h2Pol, fPol, tPol, 0, (uint64_t *) ptr_extra_mem, 3 * N);
        }
    }

    std::shared_ptr<HintHandler> H1H2HintHandlerBuilder::build() const
    {
        return std::make_unique<H1H2HintHandler>();
    }
}