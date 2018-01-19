
void PowerUp(CMat & Grid, const CPosition & Pos)
{
    for(unsigned i=0; i < 5; ++i)
    {
        if((Pos.first - 2 + i) >= 0 && (Pos.first - 2 + i) < (Grid.size()))
        {
            Grid[Pos.first-2+i][Pos.second]=0;
        }
    }

    for(unsigned i=0; i < 5; ++i)
    {
        if((Pos.second - 2 + i) >= 0 && (Pos.second - 2 + i) < (Grid.size()))
        {
            Grid[Pos.first][Pos.second-2+i]=0;
        }
    }

    DisplayGrid (Grid);
}