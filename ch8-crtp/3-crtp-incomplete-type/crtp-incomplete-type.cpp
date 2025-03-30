template <typename D>
class B
{
public:
  // no type named 'T' in 'D'
  // using T = typename D::T;

  // T *m_p{nullptr};
};

class D : public B<D>
{
public:
  using T = int;
};

int main()
{
  D d;
  return 0;
}
