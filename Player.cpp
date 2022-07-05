#include "Player.h"

Player::Player(int x, int y, const sf::Texture &activeTexture, const sf::Texture &inactiveTexture, std::vector<Attack> &attacks)
    : Unit{x, y, activeTexture, inactiveTexture, true}, m_maxAttackPoints{2}, m_currentAttackPoints{m_maxAttackPoints}, m_minAttackRange{1}, m_viewDistance{simpleConst::tileWidthi*6}
{
    m_attacks = &attacks;
    m_attackMaxRanges = std::vector<int>(m_currentAttackPoints + 1, 0);
}

void Player::updateRanges()
{
    int cost = 0;
    for (auto const &attack : *m_attacks)
    {
        cost = attack.getCost();
        if (cost <= m_currentAttackPoints)
        {
            if (attack.getMinRange() < m_minAttackRange)
            {
                m_minAttackRange = attack.getMinRange();
            }
            if (attack.getMaxRange() > m_attackMaxRanges[cost])
            {
                m_attackMaxRanges[cost] = attack.getMaxRange();
            }
        }
    }
}

bool Player::chooseAttack(sf::Vector2f clickPosition)
{
    for (auto const &attack : *m_attacks)
    {
        if (attack.isValid(m_currentAttackPoints, getDistanceFromTarget(*m_target)) &&
            attack.getGlobalBounds().contains(clickPosition))
        {
            m_isAttacking = true;
            m_selectedAttack = &attack;
            return true;
        }
    }
    return false;
}

void Player::stopAttack()
{
    m_isAttacking = false;
    m_selectedAttack = nullptr;
}

AttackResult Player::finishAttack(int attackRoll)
{
    AttackResult result;
    m_currentAttackPoints -= m_selectedAttack->getCost();
    if (attackRoll > m_target->getDefense())
    {
        int xpReceived = m_target->takeDamage(m_selectedAttack->getDamage());
        m_experiencePoints += xpReceived;
        if (m_target->isAlive())
        {
            result = AttackResult::Hit;
        }
        else
        {
            result = AttackResult::Kill;
        }
        if (m_experiencePoints >= c_xpToLevel[m_level - 1])
        {
            // todo: gain level if enough XP gained
        }
    }
    else
    {
        result = AttackResult::Miss;
    }
    stopAttack();
    clearTarget();

    return result;
}

int Player::getDistanceFromTarget(Unit &target) const
{
    return abs(getXCoord() - target.getXCoord()) + abs(getYCoord() - target.getYCoord());
}

void Player::setTarget(Unit *target)
{
    m_target = target;
    m_hasTarget = true;
}

void Player::clearTarget()
{
    m_target = nullptr;
    m_hasTarget = false;
}

void Player::startTurn()
{
    Unit::startTurn();
    m_currentAttackPoints = m_maxAttackPoints;
    updateRanges();
    clearTarget();
}

void Player::endTurn()
{
    Unit::endTurn();
    clearTarget();
    stopAttack();
}

void Player::draw(sf::RenderWindow &window) const
{
    window.draw(*this);
    if (m_hasTarget && !m_isAttacking)
    {
        int targetDistance = getDistanceFromTarget(*m_target);
        int j = 0;
        for (auto &&attack : *m_attacks)
        {
            if (attack.isValid(m_currentAttackPoints, targetDistance))
            {
                attack.setPosition(getPosition().x - (simpleConst::attackTileWidthf * (++j)), getPosition().y - simpleConst::attackTileWidthf);
                window.draw(attack);
            }
        }
    }
}